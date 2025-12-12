module {
  func.func @main() {
    %0 = subop.execution_group (){
      %1 = subop.get_external "{ \22table\22: \22studenten\22, \22mapping\22: { \22matrnr$2\22 :\22matrnr\22,\22name$1\22 :\22name\22,\22semester$1\22 :\22semester\22}, \22restrictions\22: [{\22cmp\22:\22isnotnull\22,\22column\22:\22matrnr\22,\22value\22:0},{\22cmp\22:\22isnotnull\22,\22column\22:\22matrnr\22,\22value\22:0}]}" : !subop.table<[matrnr$2 : i64, name$1 : !db.string, semester$1 : i64], true> loc(#loc3)
      %2 = subop.scan %1 : !subop.table<[matrnr$2 : i64, name$1 : !db.string, semester$1 : i64], true> {matrnr$2 => @s::@matrnr__notnull({type = i64}), name$1 => @s::@name({type = !db.string}), semester$1 => @s::@semester({type = i64})} loc(#loc4)
      %3 = subop.get_external "{ \22table\22: \22hoeren\22, \22mapping\22: { \22matrnr$3\22 :\22matrnr\22,\22vorlnr$1\22 :\22vorlnr\22}, \22restrictions\22: []}" : !subop.table<[matrnr$3 : i64, vorlnr$1 : i64], false> loc(#loc5)
      %4 = subop.scan %3 : !subop.table<[matrnr$3 : i64, vorlnr$1 : i64], false> {matrnr$3 => @h::@matrnr({type = i64}), vorlnr$1 => @h::@vorlnr({type = i64})} loc(#loc6)
      %5 = subop.create !subop.buffer<[link$0 : !util.ref<i8>, hash$0 : index, member$0 : i64, member$1 : i64, member$2 : !db.string]> loc(#loc7)
      //-----------------Build Side--------------------
      //Calculate hash value for s.matrnr
      %6 = subop.map %2 computes : [@hj::@hash({type = index}),@hj_u_1::@link({type = !util.ref<i8>})] input : [@s::@matrnr__notnull] (%arg0: i64){
        %14 = db.hash %arg0 : i64 loc(#loc7)
        %15 = util.invalid_ref <i8> loc(#loc7)
        tuples.return %14, %15 : index, !util.ref<i8> loc(#loc7)
      } loc(#loc7)
      //Materialize s.*
      subop.materialize %6 {@s::@semester => member$1, @s::@name => member$2, @s::@matrnr__notnull => member$0, @hj::@hash => hash$0, @hj_u_1::@link => link$0}, %5 : !subop.buffer<[link$0 : !util.ref<i8>, hash$0 : index, member$0 : i64, member$1 : i64, member$2 : !db.string]> loc(#loc7)
      //Insert into index struct using hash index and members s.*
      %7 = subop.create_hash_indexed_view %5 : <[link$0 : !util.ref<i8>, hash$0 : index, member$0 : i64, member$1 : i64, member$2 : !db.string]> hash("hash$0") link("link$0") -> <[hash$0 : index], [member$0 : i64, member$1 : i64, member$2 : !db.string], false> loc(#loc7)

      //----------------------Probe side--------------------      
      //Calculate hash value for h.matrnr
      %8 = subop.map %4 computes : [@hj_u_2::@hash({type = index})] input : [@h::@matrnr] (%arg0: i64){
        %14 = db.hash %arg0 : i64 loc(#loc7)
        tuples.return %14 : index loc(#loc7)
      } loc(#loc7)
      
      //Add some kind of selection???????? here or where
      %9 = subop.lookup %8%7 [@hj_u_2::@hash] : !subop.hash_indexed_view<[hash$0 : index], [member$0 : i64, member$1 : i64, member$2 : !db.string], false> @lookup_u_3::@list({type = !subop.list<!subop.lookup_entry_ref<!subop.hash_indexed_view<[hash$0 : index], [member$0 : i64, member$1 : i64, member$2 : !db.string], false>>>}) loc(#loc8)
      %10 = subop.nested_map %9 [@lookup_u_3::@list] (%arg0, %arg1) {
        %14 = subop.scan_list %arg1 : !subop.list<!subop.lookup_entry_ref<!subop.hash_indexed_view<[hash$0 : index], [member$0 : i64, member$1 : i64, member$2 : !db.string], false>>> @lookup_u_1_::@entryref({type = !subop.lookup_entry_ref<!subop.hash_indexed_view<[hash$0 : index], [member$0 : i64, member$1 : i64, member$2 : !db.string], false>>}) loc(#loc10)
        %15 = subop.combine_tuple %14, %arg0 loc(#loc7)
        %16 = subop.gather %15 @lookup_u_1_::@entryref {member$0 => @lookup_u_4::@member$0({type = i64})} loc(#loc7)
        %17 = subop.map %16 computes : [@lookup_u_2::@pred({type = i1})] input : [@lookup_u_4::@member$0,@h::@matrnr] (%arg2: i64,%arg3: i64){
          %21 = db.compare eq %arg2 : i64, %arg3 : i64 loc(#loc11)
          tuples.return %21 : i1 loc(#loc12)
        } loc(#loc7)
        %18 = subop.filter %17 all_true [@lookup_u_2::@pred] loc(#loc7)
        %19 = subop.gather %18 @lookup_u_1_::@entryref {member$1 => @s::@semester({type = i64}), member$2 => @s::@name({type = !db.string}), member$0 => @s::@matrnr__notnull({type = i64})} loc(#loc13)
        %20 = subop.combine_tuple %19, %arg0 loc(#loc14)
        tuples.return %20 : !tuples.tuplestream loc(#loc15)
      } loc(#loc9)
      %11 = subop.map %10 computes : [@s::@matrnr({type = !db.nullable<i64>})] input : [@s::@matrnr__notnull] (%arg0: i64){
        %14 = db.as_nullable %arg0 : i64 -> <i64> loc(#loc17)
        tuples.return %14 : !db.nullable<i64> loc(#loc18)
      } loc(#loc16)
      %12 = subop.create !subop.result_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64]> loc(#loc19)
      subop.materialize %11 {@s::@matrnr => matrnr$0, @s::@name => name$0, @s::@semester => semester$0, @h::@matrnr => matrnr$1, @h::@vorlnr => vorlnr$0}, %12 : !subop.result_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64]> loc(#loc20)
      %13 = subop.create_from ["matrnr", "name", "semester", "matrnr", "vorlnr"] %12 : !subop.result_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64]> -> !subop.local_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64], ["matrnr", "name", "semester", "matrnr", "vorlnr"]> loc(#loc21)
      subop.execution_group_return %13 : !subop.local_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64], ["matrnr", "name", "semester", "matrnr", "vorlnr"]> loc(#loc22)
    } -> !subop.local_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64], ["matrnr", "name", "semester", "matrnr", "vorlnr"]> loc(#loc2)
    subop.set_result 0 %0 : !subop.local_table<[matrnr$0 : !db.nullable<i64>, name$0 : !db.string, semester$0 : i64, matrnr$1 : i64, vorlnr$0 : i64], ["matrnr", "name", "semester", "matrnr", "vorlnr"]> loc(#loc23)
    return loc(#loc24)
  } loc(#loc1)
} loc(#loc)
#loc = loc("./snapshots//detailed-snapshot-27.mlir":1:0)
#loc1 = loc("./snapshots//detailed-snapshot-27.mlir":2:2)
#loc2 = loc("./snapshots//detailed-snapshot-27.mlir":3:4)
#loc3 = loc("./snapshots//detailed-snapshot-27.mlir":4:6)
#loc4 = loc("./snapshots//detailed-snapshot-27.mlir":5:6)
#loc5 = loc("./snapshots//detailed-snapshot-27.mlir":6:6)
#loc6 = loc("./snapshots//detailed-snapshot-27.mlir":7:6)
#loc7 = loc("./snapshots//detailed-snapshot-27.mlir":8:6)
#loc8 = loc("./snapshots//detailed-snapshot-27.mlir":13:6)
#loc9 = loc("./snapshots//detailed-snapshot-27.mlir":17:6)
#loc10 = loc("./snapshots//detailed-snapshot-27.mlir":18:8)
#loc11 = loc("./snapshots//detailed-snapshot-27.mlir":14:8)
#loc12 = loc("./snapshots//detailed-snapshot-27.mlir":15:8)
#loc13 = loc("./snapshots//detailed-snapshot-27.mlir":19:8)
#loc14 = loc("./snapshots//detailed-snapshot-27.mlir":20:8)
#loc15 = loc("./snapshots//detailed-snapshot-27.mlir":21:8)
#loc16 = loc("./snapshots//detailed-snapshot-27.mlir":23:6)
#loc17 = loc("./snapshots//detailed-snapshot-27.mlir":24:8)
#loc18 = loc("./snapshots//detailed-snapshot-27.mlir":25:8)
#loc19 = loc("./snapshots//detailed-snapshot-27.mlir":27:6)
#loc20 = loc("./snapshots//detailed-snapshot-27.mlir":28:6)
#loc21 = loc("./snapshots//detailed-snapshot-27.mlir":29:6)
#loc22 = loc("./snapshots//detailed-snapshot-27.mlir":30:6)
#loc23 = loc("./snapshots//detailed-snapshot-27.mlir":32:4)
#loc24 = loc("./snapshots//detailed-snapshot-27.mlir":33:4)
