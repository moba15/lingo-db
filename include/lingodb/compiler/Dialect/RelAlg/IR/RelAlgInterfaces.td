#ifndef Interfaces
#define Interfaces
include "mlir/IR/OpBase.td"
def Relalg_CmpOpInterface : OpInterface<"CmpOpInterface"> {
  let cppNamespace = "::lingodb::compiler::dialect::relalg";

  let methods = [
    InterfaceMethod<
      /*description=*/"Returns true if it is Equality Predicate",
      /*retTy=*/"bool",
      /*methodName=*/"isEqualityPred"
      ,(ins "bool":$nullsAreEqual)
    >,
    InterfaceMethod<
      /*description=*/"Returns true if it is UnEquality Predicate",
      /*retTy=*/"bool",
      /*methodName=*/"isUnequalityPred"
    >,
    InterfaceMethod<
      /*description=*/"Returns true if it is Equality Predicate",
      /*retTy=*/"bool",
      /*methodName=*/"isLessPred"
      ,(ins "bool":$eq)
    >,
    InterfaceMethod<
      /*description=*/"Returns true if it is Equality Predicate",
      /*retTy=*/"bool",
      /*methodName=*/"isGreaterPred"
      ,(ins "bool":$eq)
    >,
    InterfaceMethod<
      /*description=*/"Returns true if it is Equality Predicate",
      /*retTy=*/"mlir::Value",
      /*methodName=*/"getLeft"
    >,
    InterfaceMethod<
      /*description=*/"Returns true if it is Equality Predicate",
      /*retTy=*/"mlir::Value",
      /*methodName=*/"getRight"
    >,
  ];
}

def Operator : OpInterface<"Operator"> {
let methods = [
InterfaceMethod<
        "get all columns that are used directly by this operator",
        "lingodb::compiler::dialect::relalg::ColumnSet", "getUsedColumns",(ins ),"", /*defaultImplementation=*/[{
    return lingodb::compiler::dialect::relalg::detail::getUsedColumns(this->getOperation());
}]
>,
InterfaceMethod<
        "get all columns that are available after this operator",
        "lingodb::compiler::dialect::relalg::ColumnSet", "getAvailableColumns",(ins ),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::getAvailableColumns(this->getOperation());
}]
>,
InterfaceMethod<
        "get all columns that are available after this operator",
        "bool", "canColumnReach",(ins "Operator":$source, "Operator":$target,"const lingodb::compiler::dialect::tuples::Column*":$column),"", /*defaultImplementation=*/[{
        return lingodb::compiler::dialect::relalg::detail::canColumnReach(this->getOperation(),source,target,column);
}]
>,
InterfaceMethod<
        "get all functional dependencies that hold after this operator",
        "lingodb::compiler::dialect::relalg::FunctionalDependencies", "getFDs",(ins ),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::getFDs(this->getOperation());
}]
>,

InterfaceMethod<
        "",
        "void", "moveSubTreeBefore",(ins "Operator":$before),"", /*defaultImplementation=*/[{
lingodb::compiler::dialect::relalg::detail::moveSubTreeBefore(this->getOperation(),before);
}]
>,
InterfaceMethod<
        "get children of operator",
        "llvm::SmallVector<Operator,4>", "getChildren",(ins ),"", /*defaultImplementation=*/[{
    llvm::SmallVector<Operator,4> children;
    auto op=this->getOperation();
    for (auto operand : op->getOperands()) {
        mlir::Type t=operand.getType();
        bool isreltype=mlir::isa<lingodb::compiler::dialect::tuples::TupleStreamType>(t);
        auto childOp=operand.getDefiningOp();
        if (isreltype&&childOp) {
            auto childOperator=mlir::dyn_cast_or_null<Operator>(childOp);
            if(childOperator){
                    children.push_back(childOperator);
            }
        }
    }
    return children;
}]
>,
InterfaceMethod<
        "get children of operator",
        "llvm::SmallVector<Operator,4>", "getAllSubOperators",(ins ),"", /*defaultImplementation=*/[{
    llvm::SmallVector<Operator,4> suboperators;
    suboperators.push_back(mlir::dyn_cast_or_null<Operator>(this->getOperation()));
    auto children=this->getChildren();
    for (auto child : children) {
        auto subsub=child.getAllSubOperators();
        suboperators.insert(suboperators.end(),subsub.begin(),subsub.end());
    }
    return suboperators;
}]
>,
InterfaceMethod<
        "set children of operator",
        "void ", "setChildren",(ins "llvm::SmallVector<Operator,4>":$children),"", /*defaultImplementation=*/[{
    auto op=this->getOperation();
    size_t i=0;
    size_t j=0;
    for (auto operand : op->getOperands()) {
        mlir::Type t=operand.getType();
        bool isreltype=mlir::isa<lingodb::compiler::dialect::tuples::TupleStreamType>(t);
        auto childOp=operand.getDefiningOp();
        if (isreltype&&childOp) {
            op->setOperand(i,children[j].asRelation());
            j++;
        }
        i++;
    }
}]
>,
InterfaceMethod<
        "get resulting relation of operator",
        "mlir::Value", "asRelation",(ins ),"", /*defaultImplementation=*/[{
return this->getOperation()->getResult(0);
}]
>,
InterfaceMethod<
        "get all 'free' columns",
        "lingodb::compiler::dialect::relalg::ColumnSet", "getFreeColumns",(ins ),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::getFreeColumns(this->getOperation());
}]
>,
InterfaceMethod<
        "get all columns that are created by this operator",
        "lingodb::compiler::dialect::relalg::ColumnSet", "getCreatedColumns",(ins ),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::ColumnSet();
}]
>
];
}
def BinaryOperator : OpInterface<"BinaryOperator"> {
let methods = [
InterfaceMethod<
        "get all attributes that are used directly by this operator",
        "mlir::Operation*", "leftChild",(ins ),"", /*defaultImplementation=*/[{
return this->getOperation()->getOperand(0).getDefiningOp();
}]
>,
InterfaceMethod<
        "assoc",
        "bool", "isAssoc",(ins "BinaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::assoc.contains(lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(binop.getOperation()));
}]
>,
InterfaceMethod<
"assoc",
"bool", "isLAsscom",(ins "BinaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::lAsscom.contains(lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(binop.getOperation()));
}]
>,
InterfaceMethod<
"assoc",
"bool", "isRAsscom",(ins "BinaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::rAsscom.contains(lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(binop.getOperation()));
}]
>,
InterfaceMethod<
        "get all attributes that are used directly by this operator",
        "mlir::Operation*", "rightChild",(ins ),"", /*defaultImplementation=*/[{
return this->getOperation()->getOperand(1).getDefiningOp();
}]
>
];
}
def UnaryOperator : OpInterface<"UnaryOperator"> {
let methods = [
InterfaceMethod<
        "get all attributes that are used directly by this operator",
        "mlir::Operation*", "child",(ins ),"", /*defaultImplementation=*/[{
return this->getOperation()->getOperand(0).getDefiningOp();
}]
>,
InterfaceMethod<
"assoc",
"bool", "reorderable",(ins "UnaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::reorderable.contains(lingodb::compiler::dialect::relalg::detail::getUnaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getUnaryOperatorType(binop.getOperation()));
}]
>,
InterfaceMethod<
"assoc",
"bool", "lPushable",(ins "BinaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::lPushable.contains(lingodb::compiler::dialect::relalg::detail::getUnaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(binop.getOperation()));
}]
>,
InterfaceMethod<
"assoc",
"bool", "rPushable",(ins "BinaryOperator":$binop),"", /*defaultImplementation=*/[{
return lingodb::compiler::dialect::relalg::detail::rPushable.contains(lingodb::compiler::dialect::relalg::detail::getUnaryOperatorType(this->getOperation()), lingodb::compiler::dialect::relalg::detail::getBinaryOperatorType(binop.getOperation()));
}]
>
];
}
def TupleLamdaOperator : OpInterface<"TupleLamdaOperator"> {
let methods = [
InterfaceMethod<
        "",
        "mlir::Region&", "getLambdaRegion",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0);
}]
>,
InterfaceMethod<
        "",
        "mlir::Block&", "getLambdaBlock",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0).front();
}]>,
InterfaceMethod<
        "",
        "mlir::BlockArgument", "getLambdaArgument",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0).getArgument(0);
}]
>];
}
def PredicateOperator : OpInterface<"PredicateOperator"> {
let methods = [
InterfaceMethod<
        "",
        "mlir::Region&", "getPredicateRegion",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0);
}]
>,
InterfaceMethod<
        "",
        "mlir::Block&", "getPredicateBlock",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0).front();
}]>,
InterfaceMethod<
        "",
        "void", "addPredicate",(ins "std::function<mlir::Value(mlir::Value, mlir::OpBuilder& builder)>":$producer),""
        , /*op=*/[{
lingodb::compiler::dialect::relalg::detail::addPredicate(this->getOperation(),producer);
}]>,
InterfaceMethod<
        "",
        "mlir::BlockArgument", "getPredicateArgument",(ins ),""
        , /*op=*/[{
return this->getOperation()->getRegion(0).getArgument(0);
}]
>,
InterfaceMethod<
        "",
        "void", "initPredicate",(ins ),""
        , /*op=*/[{
lingodb::compiler::dialect::relalg::detail::initPredicate(this->getOperation());
}]
>];
}

def ColumnFoldable : OpInterface<"ColumnFoldable"> {
let methods = [
InterfaceMethod<"","mlir::LogicalResult", "foldColumns",(ins "lingodb::compiler::dialect::relalg::ColumnFoldInfo&":$columnInfo),"",[{return mlir::failure();}]>,
InterfaceMethod<"","mlir::LogicalResult", "eliminateDeadColumns",(ins "lingodb::compiler::dialect::relalg::ColumnSet&":$usedColumns,"mlir::Value&":$newStream),"",[{return mlir::failure();}]>
];
}

#endif// Interfaces
