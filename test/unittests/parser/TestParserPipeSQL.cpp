#include "lingodb/compiler/frontend/driver.h"
#include "lingodb/runtime/LingoDBHashIndex.h"

#include <catch2/catch_test_macros.hpp>
namespace {

TEST_CASE("Parser:simplePipeSelect") {
   std::string sql{":select * from test "
                   "|> where sf<3 "
                   "|> where sf<4 "
                   "|> select t.t"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"PIPE SELECT NODE\"];"
                        "node1 [label=\"SelectNode\"];"
                        "node1 -> node2 [label=\"select list\"];"
                        "node2 [label=\"Targets List\"];"
                        "node2 -> node3 [label=\"target 1\"];"
                        "node3 [label=\"*\"];"
                        "node1 -> node4 [label=\"FROM\"];"
                        "node4 [label=\"BaseTable\\n table: test\\nschema: \\ncatalog: \"];"
                        "node0 -> node1 [label=\"start\"];"
                        "node5 [label=\"σ<\"];"
                        "node5 -> node6 [label=\"left\"];"
                        "node6 [label=\"ColumnRef\\nsf\"];"
                        "node5 -> node7 [label=\"right\"];"
                        "node7 [label=\"Constant\\n3\"];"
                        "node1 -> node5 [label=\"next\"];"
                        "node8 [label=\"σ<\"];"
                        "node8 -> node9 [label=\"left\"];"
                        "node9 [label=\"ColumnRef\\nsf\"];"
                        "node8 -> node10 [label=\"right\"];"
                        "node10 [label=\"Constant\\n4\"];"
                        "node5 -> node8 [label=\"next\"];"
                        "node11 [label=\"Targets List\"];"
                        "node11 -> node12 [label=\"target 1\"];"
                        "node12 [label=\"ColumnRef\\nt.t\"];"
                        "node8 -> node11 [label=\"next\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected);
}
TEST_CASE("Parser:simplePipeJoins") {
   std::string sql{":FROM studenten as s "
                   "|> where s.semester>2 or s.semester>1 "
                   "|> join hoeren as h on h.matrnr=s.matrnr "
                   "|> join vorlesung as v on v.vorlnr=h.vorlnr "
                   "|> join professoren as p on p.persnr=v.gelesenVon "
                   "|> where s.semester>=4 and s.semester>=3 "
                   "|> order by s.semester DESC"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"PIPE SELECT NODE\"];"
                        "node1 [label=\"JoinReftype: inner JOIN\\n\"];"
                        "node1 -> node2 [label=\"left\"];"
                        "node2 [label=\"PIPE SELECT NODE\"];"
                        "node3 [label=\"JoinReftype: inner JOIN\\n\"];"
                        "node3 -> node4 [label=\"left\"];"
                        "node4 [label=\"PIPE SELECT NODE\"];"
                        "node5 [label=\"JoinReftype: inner JOIN\\n\"];"
                        "node5 -> node6 [label=\"left\"];"
                        "node6 [label=\"PIPE SELECT NODE\"];"
                        "node7 [label=\"BaseTable\\n table: studenten\\nschema: \\ncatalog: \\nalias: s\"];"
                        "node6 -> node7 [label=\"start\"];"
                        "node8 [label=\"σOR\"];"
                        "node8 -> node9 [label=\"child 1\"];"
                        "node9 [label=\"σ>\"];"
                        "node9 -> node10 [label=\"left\"];"
                        "node10 [label=\"ColumnRef\\ns.semester\"];"
                        "node9 -> node11 [label=\"right\"];"
                        "node11 [label=\"Constant\\n2\"];"
                        "node8 -> node12 [label=\"child 2\"];"
                        "node12 [label=\"σ>\"];"
                        "node12 -> node13 [label=\"left\"];"
                        "node13 [label=\"ColumnRef\\ns.semester\"];"
                        "node12 -> node14 [label=\"right\"];"
                        "node14 [label=\"Constant\\n1\"];"
                        "node7 -> node8 [label=\"next\"];"
                        "node5 -> node15 [label=\"right\"];"
                        "node15 [label=\"BaseTable\\n table: hoeren\\nschema: \\ncatalog: \\nalias: h\"];"
                        "node5 -> node16 [label=\"ON\"];"
                        "node16 [label=\"σ=\"];"
                        "node16 -> node17 [label=\"left\"];"
                        "node17 [label=\"ColumnRef\\nh.matrnr\"];"
                        "node16 -> node18 [label=\"right\"];"
                        "node18 [label=\"ColumnRef\\ns.matrnr\"];"
                        "node4 -> node5 [label=\"start\"];"
                        "node3 -> node19 [label=\"right\"];"
                        "node19 [label=\"BaseTable\\n table: vorlesung\\nschema: \\ncatalog: \\nalias: v\"];"
                        "node3 -> node20 [label=\"ON\"];"
                        "node20 [label=\"σ=\"];"
                        "node20 -> node21 [label=\"left\"];"
                        "node21 [label=\"ColumnRef\\nv.vorlnr\"];"
                        "node20 -> node22 [label=\"right\"];"
                        "node22 [label=\"ColumnRef\\nh.vorlnr\"];"
                        "node2 -> node3 [label=\"start\"];"
                        "node1 -> node23 [label=\"right\"];"
                        "node23 [label=\"BaseTable\\n table: professoren\\nschema: \\ncatalog: \\nalias: p\"];"
                        "node1 -> node24 [label=\"ON\"];"
                        "node24 [label=\"σ=\"];"
                        "node24 -> node25 [label=\"left\"];"
                        "node25 [label=\"ColumnRef\\np.persnr\"];"
                        "node24 -> node26 [label=\"right\"];"
                        "node26 [label=\"ColumnRef\\nv.gelesenvon\"];"
                        "node0 -> node1 [label=\"start\"];"
                        "node27 [label=\"σAND\"];"
                        "node27 -> node28 [label=\"child 1\"];"
                        "node28 [label=\"σ>=\"];"
                        "node28 -> node29 [label=\"left\"];"
                        "node29 [label=\"ColumnRef\\ns.semester\"];"
                        "node28 -> node30 [label=\"right\"];"
                        "node30 [label=\"Constant\\n4\"];"
                        "node27 -> node31 [label=\"child 2\"];"
                        "node31 [label=\"σ>=\"];"
                        "node31 -> node32 [label=\"left\"];"
                        "node32 [label=\"ColumnRef\\ns.semester\"];"
                        "node31 -> node33 [label=\"right\"];"
                        "node33 [label=\"Constant\\n3\"];"
                        "node1 -> node27 [label=\"next\"];"
                        "node34 [label=\"ORDER BY\"];"
                        "node34 -> node35 [label=\"1: DESC\"];"
                        "node35 [label=\"ColumnRef\\ns.semester\"];"
                        "node27 -> node34 [label=\"next\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected);
}
TEST_CASE("Parser:simplePipeAggregation") {
   std::string sql{":FROM studenten as s "
                   "|> where s.semester>2 or s.semester>1 "
                   "|> AGGREGATE max(s.semster), min(s.semester) GROUP BY s.matrnr "
                   "|> Order by zesz"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"PIPE SELECT NODE\"];"
                        "node1 [label=\"BaseTable\\n table: studenten\\nschema: \\ncatalog: \\nalias: s\"];"
                        "node0 -> node1 [label=\"start\"];"
                        "node2 [label=\"σOR\"];"
                        "node2 -> node3 [label=\"child 1\"];"
                        "node3 [label=\"σ>\"];"
                        "node3 -> node4 [label=\"left\"];"
                        "node4 [label=\"ColumnRef\\ns.semester\"];"
                        "node3 -> node5 [label=\"right\"];"
                        "node5 [label=\"Constant\\n2\"];"
                        "node2 -> node6 [label=\"child 2\"];"
                        "node6 [label=\"σ>\"];"
                        "node6 -> node7 [label=\"left\"];"
                        "node7 [label=\"ColumnRef\\ns.semester\"];"
                        "node6 -> node8 [label=\"right\"];"
                        "node8 [label=\"Constant\\n1\"];"
                        "node1 -> node2 [label=\"next\"];"
                        "node9 [label=\"Aggregation\"];"
                        "node9 -> node10 [label=\"agg_0\"];"
                        "node10 [label=\"Functionname: maxagg\"];"
                        "node10 -> node11 [label=\"arg 1\"];"
                        "node11 [label=\"ColumnRef\\ns.semster\"];"
                        "node9 -> node12 [label=\"agg_1\"];"
                        "node12 [label=\"Functionname: minagg\"];"
                        "node12 -> node13 [label=\"arg 1\"];"
                        "node13 [label=\"ColumnRef\\ns.semester\"];"
                        "node9 -> node14 [label=\"group_by\"];"
                        "node14 [label=\"GROUP BY\"];"
                        "node14 -> node15 [label=\"expr 1\"];"
                        "node15 [label=\"ColumnRef\\ns.matrnr\"];"
                        "node2 -> node9 [label=\"next\"];"
                        "node16 [label=\"ORDER BY\"];"
                        "node16 -> node17 [label=\"1: ASC\"];"
                        "node17 [label=\"ColumnRef\\nzesz\"];"
                        "node9 -> node16 [label=\"next\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected);
}

} // namespace