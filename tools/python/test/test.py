import lingodb

import pandas as pd
import pyarrow as pa
import os

if not os.path.exists("./resources/data/uni"):
    os.makedirs("./resources/data/uni")
    con_create=lingodb.connect_to_db("./resources/data/uni")
    with open("./resources/sql/uni/initialize.sql") as f:
        # line for line
        for line in f:
            con_create.sql_stmt(line)

con = lingodb.connect_to_db("./resources/data/uni")
print(con.sql("""
-- all students who never attended a lecture
select * from studenten s
where not exists(select * from hoeren h where h.matrnr=s.matrnr)
""").to_pandas())
print(con.sql("select 1").to_pandas())

df = pd.DataFrame(data={'col1': [1, 2, 3, 4], 'col2': ["foo", "foo", "bar", "bar"]})

con2 = lingodb.create_in_memory()
con2.add_table("df", pa.Table.from_pandas(df))
print(con2.mlir("""module {
  func.func @main() {
    %result = relalg.query (){
    %0 = relalg.basetable {table_identifier = "df"} columns: { col1 => @df::@col1({type = i64})}
    %1 = relalg.selection %0 (%arg0 : !tuples.tuple){
        %4 = db.constant(2) : i64
        %5 = tuples.getcol %arg0 @df::@col1 : i64
        %6 = db.compare gt %5 : i64, %4: i64
        tuples.return %6 : i1
    }
    %2 = relalg.aggregation %1 [] computes : [@aggr0::@tmp_attr0({type = i64})] (%arg0: !tuples.tuplestream,%arg1: !tuples.tuple){
      %4 = relalg.count %arg0
      tuples.return %4 : i64
    }
    %3 = relalg.materialize %2 [@aggr0::@tmp_attr0] => ["count"] : !subop.local_table<[count: i64],["cnt"]>
    relalg.query_return %3 : !subop.local_table<[count: i64],["cnt"]>
    } -> !subop.local_table<[count: i64],["cnt"]>
    subop.set_result 0 %result :  !subop.local_table<[count: i64],["cnt"]>
    return
  }
}
""").to_pandas())
print(con2.sql("select count(*) as c1,count(distinct col2) as c2 from df where col1>2").to_pandas())
