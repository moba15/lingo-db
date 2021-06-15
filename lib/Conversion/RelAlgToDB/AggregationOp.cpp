#include "mlir/Conversion/RelAlgToDB/ProducerConsumerNode.h"
#include "mlir/Dialect/DB/IR/DBOps.h"
#include "mlir/Dialect/RelAlg/IR/RelAlgOps.h"
#include "mlir/Dialect/util/UtilOps.h"

class AggregationLowering : public mlir::relalg::ProducerConsumerNode {
   mlir::relalg::AggregationOp aggregationOp;
   size_t builderId;

   mlir::TupleType keyTupleType;
   mlir::TupleType valTupleType;
   mlir::TupleType insertEntryType;

   std::vector<const mlir::relalg::RelationalAttribute*> keyAttributes;
   std::vector<const mlir::relalg::RelationalAttribute*> valAttributes;
   std::unordered_map<const mlir::relalg::RelationalAttribute*, size_t> keyMapping;

   public:
   AggregationLowering(mlir::relalg::AggregationOp aggregationOp) : mlir::relalg::ProducerConsumerNode(aggregationOp.rel()), aggregationOp(aggregationOp) {
   }
   virtual void setInfo(mlir::relalg::ProducerConsumerNode* consumer, mlir::relalg::Attributes requiredAttributes) override {
      this->consumer = consumer;
      this->requiredAttributes = requiredAttributes;
      this->requiredAttributes.insert(aggregationOp.getUsedAttributes());
      propagateInfo();
   }
   virtual mlir::relalg::Attributes getAvailableAttributes() override {
      return aggregationOp.getAvailableAttributes();
   }
   virtual void consume(mlir::relalg::ProducerConsumerNode* child, mlir::relalg::ProducerConsumerBuilder& builder, mlir::relalg::LoweringContext& context) override {
      std::vector<mlir::Value> keys, values;
      for (const auto* attr : keyAttributes) {
         keys.push_back(context.getValueForAttribute(attr));
      }
      for (const auto* attr : valAttributes) {
         values.push_back(context.getValueForAttribute(attr));
      }
      mlir::Value htBuilder = context.builders[builderId];
      mlir::Value packedKey;
      if(keyTupleType.getTypes().size()==0){
         packedKey=builder.create<mlir::util::UndefTupleOp>(aggregationOp->getLoc(), keyTupleType);
      }else{
         packedKey=builder.create<mlir::util::PackOp>(aggregationOp->getLoc(), keyTupleType, keys);
      }
      mlir::Value packedVal = builder.create<mlir::util::PackOp>(aggregationOp->getLoc(), valTupleType, values);
      mlir::Value packed = builder.create<mlir::util::PackOp>(aggregationOp->getLoc(), insertEntryType, mlir::ValueRange({packedKey, packedVal}));

      mlir::Value mergedBuilder = builder.create<mlir::db::BuilderMerge>(aggregationOp->getLoc(), htBuilder.getType(), htBuilder, packed);
      context.builders[builderId] = mergedBuilder;
   }

   virtual void produce(mlir::relalg::LoweringContext& context, mlir::relalg::ProducerConsumerBuilder& builder) override {
      std::vector<mlir::Type> keyTypes;
      std::vector<mlir::Type> valTypes;
      auto scope = context.createScope();

      std::vector<std::function<std::pair<const mlir::relalg::RelationalAttribute*, mlir::Value>(mlir::ValueRange, mlir::OpBuilder & builder)>> finalizeFunctions;
      std::vector<std::function<std::vector<mlir::Value>(mlir::ValueRange, mlir::ValueRange, mlir::OpBuilder & builder)>> aggregationFunctions;
      for (auto attr : aggregationOp.group_by_attrs()) {
         if (auto attrRef = attr.dyn_cast_or_null<mlir::relalg::RelationalAttributeRefAttr>()) {
            keyTypes.push_back(attrRef.getRelationalAttribute().type);
            keyAttributes.push_back(&attrRef.getRelationalAttribute());
            keyMapping.insert({&attrRef.getRelationalAttribute(), keyTypes.size() - 1});
         }
      }
      std::vector<mlir::Value> defaultValues;
      std::vector<mlir::Type> aggrTypes;
      auto counterType = mlir::db::IntType::get(builder.getContext(), false, 64);

      aggregationOp.aggr_func().walk([&](mlir::relalg::AddAttrOp addAttrOp) {
         if (auto aggrFn = mlir::dyn_cast_or_null<mlir::relalg::AggrFuncOp>(addAttrOp.val().getDefiningOp())) {
            auto* destAttr = &addAttrOp.attr().getRelationalAttribute();
            auto* attr = &aggrFn.attr().getRelationalAttribute();
            size_t currValIdx = valAttributes.size();
            valAttributes.push_back(attr);
            valTypes.push_back(attr->type);
            mlir::Type resultingType = addAttrOp.attr().getRelationalAttribute().type;
            size_t currDestIdx = aggrTypes.size();

            if (aggrFn.fn() == mlir::relalg::AggrFunc::sum) {
               finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr](mlir::ValueRange range, mlir::OpBuilder& builder) { return std::make_pair(destAttr, range[currDestIdx]); });
               aggrTypes.push_back(resultingType);
               auto initVal = builder.create<mlir::db::NullOp>(aggregationOp.getLoc(), resultingType);
               defaultValues.push_back(initVal);
               aggregationFunctions.push_back([currDestIdx = currDestIdx, currValIdx = currValIdx, resultingType = resultingType](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
                  std::vector<mlir::Value> res;
                  mlir::Value currVal = aggr[currDestIdx];
                  mlir::Value newVal = val[currValIdx];
                  mlir::Value added = builder.create<mlir::db::AddOp>(builder.getUnknownLoc(), resultingType, currVal, newVal);
                  mlir::Value casted = builder.create<mlir::db::CastOp>(builder.getUnknownLoc(), currVal.getType(), newVal);
                  mlir::Value isNull = builder.create<mlir::db::IsNullOp>(builder.getUnknownLoc(), mlir::db::BoolType::get(builder.getContext()), aggr[currDestIdx]);
                  res.push_back(builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), isNull, casted, added));
                  return res;
               });
            } else if (aggrFn.fn() == mlir::relalg::AggrFunc::min) {
               finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr](mlir::ValueRange range, mlir::OpBuilder& builder) { return std::make_pair(destAttr, range[currDestIdx]); });
               aggrTypes.push_back(resultingType);
               auto initVal = builder.create<mlir::db::NullOp>(aggregationOp.getLoc(), resultingType);
               defaultValues.push_back(initVal);
               aggregationFunctions.push_back([currDestIdx = currDestIdx, currValIdx = currValIdx](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
                  mlir::Value currVal = aggr[currDestIdx];
                  mlir::Value newVal = val[currValIdx];
                  std::vector<mlir::Value> res;
                  mlir::Value currLtNew = builder.create<mlir::db::CmpOp>(builder.getUnknownLoc(), mlir::db::DBCmpPredicate::lt, currVal, newVal);
                  mlir::Value casted = builder.create<mlir::db::CastOp>(builder.getUnknownLoc(), currVal.getType(), newVal);
                  res.push_back(builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), currLtNew, currVal, casted));
                  return res;
               });
            } else if (aggrFn.fn() == mlir::relalg::AggrFunc::max) {
               finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr](mlir::ValueRange range, mlir::OpBuilder& builder) { return std::make_pair(destAttr, range[currDestIdx]); });
               aggrTypes.push_back(resultingType);
               auto initVal = builder.create<mlir::db::NullOp>(aggregationOp.getLoc(), resultingType);
               defaultValues.push_back(initVal);
               aggregationFunctions.push_back([currDestIdx = currDestIdx, currValIdx = currValIdx](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
                  mlir::Value currVal = aggr[currDestIdx];
                  mlir::Value newVal = val[currValIdx];
                  std::vector<mlir::Value> res;
                  mlir::Value currGtNew = builder.create<mlir::db::CmpOp>(builder.getUnknownLoc(), mlir::db::DBCmpPredicate::gt, currVal, newVal); //todo how to handle nullables, maybe db::select?
                  mlir::Value casted = builder.create<mlir::db::CastOp>(builder.getUnknownLoc(), currVal.getType(), newVal);
                  res.push_back(builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), currGtNew, currVal, casted));
                  return res;
               });
            } else if (aggrFn.fn() == mlir::relalg::AggrFunc::avg) {
               aggrTypes.push_back(resultingType);
               aggrTypes.push_back(counterType);
               auto initVal = builder.create<mlir::db::ConstantOp>(aggregationOp.getLoc(), resultingType.cast<mlir::db::DBType>().getBaseType(), builder.getI64IntegerAttr(0));
               auto initCounterVal = builder.create<mlir::db::ConstantOp>(aggregationOp.getLoc(), counterType, builder.getI64IntegerAttr(0));

               defaultValues.push_back(builder.create<mlir::db::CastOp>(aggregationOp.getLoc(), resultingType, initVal));
               defaultValues.push_back(initCounterVal);
               finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr, resultingType = resultingType](mlir::ValueRange range, mlir::OpBuilder builder) {
                  mlir::Value casted=builder.create<mlir::db::CastOp>(builder.getUnknownLoc(), resultingType, range[currDestIdx+1]);
                  mlir::Value average=builder.create<mlir::db::DivOp>(builder.getUnknownLoc(), resultingType, range[currDestIdx], casted);
                  return std::make_pair(destAttr, average); });
               aggregationFunctions.push_back([currDestIdx = currDestIdx, currValIdx = currValIdx, resultingType = resultingType, counterType = counterType](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
                  std::vector<mlir::Value> res;
                  auto one = builder.create<mlir::db::ConstantOp>(builder.getUnknownLoc(), counterType, builder.getI64IntegerAttr(1));
                  mlir::Value added1 = builder.create<mlir::db::AddOp>(builder.getUnknownLoc(), resultingType, aggr[currDestIdx], val[currValIdx]);
                  mlir::Value added2 = builder.create<mlir::db::AddOp>(builder.getUnknownLoc(), counterType, aggr[currDestIdx + 1], one);
                  mlir::Value isNull1 = builder.create<mlir::db::IsNullOp>(builder.getUnknownLoc(), mlir::db::BoolType::get(builder.getContext()), aggr[currDestIdx]);

                  res.push_back(builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), isNull1, aggr[currDestIdx], added1));
                  res.push_back(added2);

                  return res;
               });
            } else if (aggrFn.fn() == mlir::relalg::AggrFunc::count) {
               size_t currDestIdx = aggrTypes.size();
               aggrTypes.push_back(resultingType);
               auto initVal = builder.create<mlir::db::NullOp>(aggregationOp.getLoc(), resultingType);
               defaultValues.push_back(initVal);
               finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr](mlir::ValueRange range, mlir::OpBuilder& builder) { return std::make_pair(destAttr, range[currDestIdx]); });

               aggregationFunctions.push_back([currDestIdx = currDestIdx,attr=attr, currValIdx = currValIdx, counterType = counterType,resultingType=resultingType](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
                  std::vector<mlir::Value> res;
                  auto one = builder.create<mlir::db::ConstantOp>(builder.getUnknownLoc(), counterType, builder.getI64IntegerAttr(1));
                  mlir::Value value = builder.create<mlir::db::AddOp>(builder.getUnknownLoc(), resultingType, aggr[currDestIdx], one);
                  if(attr->type.isNullable()){
                     mlir::Value isNull2 = builder.create<mlir::db::IsNullOp>(builder.getUnknownLoc(), mlir::db::BoolType::get(builder.getContext()), val[currValIdx]);
                     mlir::Value tmp=builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), isNull2, aggr[currDestIdx], value );
                     value=tmp;
                  }
                  mlir::Value isNull1 = builder.create<mlir::db::IsNullOp>(builder.getUnknownLoc(), mlir::db::BoolType::get(builder.getContext()), aggr[currDestIdx]);
                 mlir::Value casted=builder.create<mlir::db::CastOp>(builder.getUnknownLoc(), resultingType,one);

                  res.push_back(builder.create<mlir::db::SelectOp>(builder.getUnknownLoc(), isNull1, casted, value));
                  return res;
               });
            }
         }
         if (auto countOp = mlir::dyn_cast_or_null<mlir::relalg::CountRowsOp>(addAttrOp.val().getDefiningOp())) {
            auto* destAttr = &addAttrOp.attr().getRelationalAttribute();
            valTypes.push_back(counterType);
            //mlir::Type resultingType = addAttrOp.attr().getRelationalAttribute().type;
            size_t currDestIdx = aggrTypes.size();
            aggrTypes.push_back(counterType);
            auto initCounterVal = builder.create<mlir::db::ConstantOp>(aggregationOp.getLoc(), counterType, builder.getI64IntegerAttr(0));
            defaultValues.push_back(initCounterVal);
            finalizeFunctions.push_back([currDestIdx = currDestIdx, destAttr = destAttr](mlir::ValueRange range, mlir::OpBuilder& builder) { return std::make_pair(destAttr, range[currDestIdx]); });

            aggregationFunctions.push_back([currDestIdx = currDestIdx, counterType = counterType](mlir::ValueRange aggr, mlir::ValueRange val, mlir::OpBuilder& builder) {
               std::vector<mlir::Value> res;
               auto one = builder.create<mlir::db::ConstantOp>(builder.getUnknownLoc(), counterType, builder.getI64IntegerAttr(1));
               mlir::Value added2 = builder.create<mlir::db::AddOp>(builder.getUnknownLoc(), counterType, aggr[currDestIdx], one);
               res.push_back(added2);
               return res;
            });
         }
      });
      auto aggrTupleType = mlir::TupleType::get(builder.getContext(), aggrTypes);
      auto initTuple = builder.create<mlir::util::PackOp>(aggregationOp->getLoc(), aggrTupleType, defaultValues);
      keyTupleType = mlir::TupleType::get(builder.getContext(), keyTypes);
      valTupleType = mlir::TupleType::get(builder.getContext(), valTypes);
      auto aggrBuilder = builder.create<mlir::db::CreateAggrHTBuilder>(aggregationOp.getLoc(), mlir::db::AggrHTBuilderType::get(builder.getContext(), keyTupleType, valTupleType), initTuple);
      mlir::Block* aggrBuilderBlock = new mlir::Block;
      aggrBuilder.region().push_back(aggrBuilderBlock);
      aggrBuilderBlock->addArguments({aggrTupleType, valTupleType});
      mlir::relalg::ProducerConsumerBuilder builder2(builder.getContext());
      builder2.setInsertionPointToStart(aggrBuilderBlock);
      auto unpackedCurr = builder2.create<mlir::util::UnPackOp>(aggregationOp->getLoc(), aggrTypes, aggrBuilderBlock->getArgument(0))->getResults();
      auto unpackedNew = builder2.create<mlir::util::UnPackOp>(aggregationOp->getLoc(), valTypes, aggrBuilderBlock->getArgument(1)).getResults();
      std::vector<mlir::Value> values;
      for (auto aggrFn : aggregationFunctions) {
         auto vec = aggrFn(unpackedCurr, unpackedNew, builder2);
         values.insert(values.end(), vec.begin(), vec.end());
      }

      mlir::Value packed = builder2.create<mlir::util::PackOp>(aggregationOp->getLoc(), aggrTupleType, values);

      builder2.create<mlir::db::YieldOp>(builder.getUnknownLoc(), packed);
      builderId = context.getBuilderId();
      context.builders[builderId] = aggrBuilder;
      this->insertEntryType = mlir::TupleType::get(builder.getContext(), {keyTupleType, valTupleType});

      auto iterEntryType = mlir::TupleType::get(builder.getContext(), {keyTupleType, aggrTupleType});
      children[0]->addRequiredBuilders({builderId});
      children[0]->produce(context, builder);
      mlir::Value hashtable = builder.create<mlir::db::BuilderBuild>(aggregationOp.getLoc(), mlir::db::AggregationHashtableType::get(builder.getContext(), keyTupleType, aggrTupleType), aggrBuilder);
      {
         auto forOp2 = builder.create<mlir::db::ForOp>(aggregationOp->getLoc(), getRequiredBuilderTypes(context), hashtable, flag, getRequiredBuilderValues(context));
         mlir::Block* block2 = new mlir::Block;
         block2->addArgument(iterEntryType);
         block2->addArguments(getRequiredBuilderTypes(context));
         forOp2.getBodyRegion().push_back(block2);
         mlir::relalg::ProducerConsumerBuilder builder2(forOp2.getBodyRegion());
         setRequiredBuilderValues(context, block2->getArguments().drop_front(1));
         auto unpacked = builder2.create<mlir::util::UnPackOp>(aggregationOp->getLoc(), iterEntryType.getTypes(), forOp2.getInductionVar()).getResults();
         mlir::ValueRange unpackedKey;
         if(!keyTypes.empty()){
            unpackedKey= builder2.create<mlir::util::UnPackOp>(aggregationOp->getLoc(), keyTypes, unpacked[0]).getResults();
         }
         auto unpackedAggr = builder2.create<mlir::util::UnPackOp>(aggregationOp->getLoc(), aggrTypes, unpacked[1]).getResults();

         for (auto fn : finalizeFunctions) {
            auto [attr, val] = fn(unpackedAggr, builder2);
            context.setValueForAttribute(scope, attr, val);
         }
         for (auto* attr : requiredAttributes) {
            if (keyMapping.count(attr)) {
               context.setValueForAttribute(scope, attr, unpackedKey[keyMapping[attr]]);
            }
         }
         consumer->consume(this, builder2, context);
         builder2.create<mlir::db::YieldOp>(aggregationOp->getLoc(), getRequiredBuilderValues(context));
         setRequiredBuilderValues(context, forOp2.results());
      }
   }
   virtual void done() override {
   }
   virtual ~AggregationLowering() {}
};

bool mlir::relalg::ProducerConsumerNodeRegistry::registeredAggregationOp = mlir::relalg::ProducerConsumerNodeRegistry::registerNode([](mlir::relalg::AggregationOp sortOp) {
   return std::make_unique<AggregationLowering>(sortOp);
});