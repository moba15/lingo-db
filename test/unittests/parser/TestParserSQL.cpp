
#include "lingodb/compiler/frontend/sql-parser/driver.h"
#include "lingodb/runtime/LingoDBHashIndex.h"

#include <catch2/catch_test_macros.hpp>
namespace {

TEST_CASE("Parser:simpleSelect") {
   std::string sql{":select s.matrnr from studenten as s where s.semester>2"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"SelectNode\"];"
      "node0 -> node1 [label=\"select list\"];"
      "node1 [label=\"Targets List\"];"
      "node1 -> node2 [label=\"target 1\"];"
      "node2 [label=\"ColumnRef\\ns.matrnr\"];"
      "node0 -> node3 [label=\"FROM\"];"
      "node3 [label=\"BaseTable\\n table: studenten\\nschema: \\ncatalog: \\nalias: s\"];"
      "node0 -> node4 [label=\"WHERE\"];"
      "node4 [label=\"σ"
      ">\"];"
      "node4 -> node5 [label=\"left\"];"
      "node5 [label=\"ColumnRef\\ns.semester\"];"
      "node4 -> node6 [label=\"right\"];"
      "node6 [label=\"Constant\\n2\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected );
}


TEST_CASE("Parser:simpleTrippleInnerJoin") {
   std::string sql{":select s.matrnr, s.name, v.titel, p.persnr, p.name "
         "from studenten as s "
         "join hoeren as h on h.matrnr=s.matrnr "
         "join vorlesungen as v on h.vorlnr=v.vorlnr "
         "join professoren as p on p.persnr=v.gelesenVon "
         "where s.semester>=4 and s.semester>=3"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"SelectNode\"];"
   "node0 -> node1 [label=\"select list\"];"
   "node1 [label=\"Targets List\"];"
   "node1 -> node2 [label=\"target 1\"];"
   "node2 [label=\"ColumnRef\\ns.matrnr\"];"
   "node1 -> node3 [label=\"target 2\"];"
   "node3 [label=\"ColumnRef\\ns.name\"];"
   "node1 -> node4 [label=\"target 3\"];"
   "node4 [label=\"ColumnRef\\nv.titel\"];"
   "node1 -> node5 [label=\"target 4\"];"
   "node5 [label=\"ColumnRef\\np.persnr\"];"
   "node1 -> node6 [label=\"target 5\"];"
   "node6 [label=\"ColumnRef\\np.name\"];"
   "node0 -> node7 [label=\"FROM\"];"
   "node7 [label=\"JoinReftype: inner JOIN\\n\"];"
   "node7 -> node8 [label=\"left\"];"
   "node8 [label=\"JoinReftype: inner JOIN\\n\"];"
   "node8 -> node9 [label=\"left\"];"
   "node9 [label=\"JoinReftype: inner JOIN\\n\"];"
   "node9 -> node10 [label=\"left\"];"
   "node10 [label=\"BaseTable\\n table: studenten\\nschema: \\ncatalog: \\nalias: s\"];"
   "node9 -> node11 [label=\"right\"];"
   "node11 [label=\"BaseTable\\n table: hoeren\\nschema: \\ncatalog: \\nalias: h\"];"
   "node9 -> node12 [label=\"ON\"];"
   "node12 [label=\"σ=\"];"
   "node12 -> node13 [label=\"left\"];"
   "node13 [label=\"ColumnRef\\nh.matrnr\"];"
   "node12 -> node14 [label=\"right\"];"
   "node14 [label=\"ColumnRef\\ns.matrnr\"];"
   "node8 -> node15 [label=\"right\"];"
   "node15 [label=\"BaseTable\\n table: vorlesungen\\nschema: \\ncatalog: \\nalias: v\"];"
   "node8 -> node16 [label=\"ON\"];"
   "node16 [label=\"σ=\"];"
   "node16 -> node17 [label=\"left\"];"
   "node17 [label=\"ColumnRef\\nh.vorlnr\"];"
   "node16 -> node18 [label=\"right\"];"
   "node18 [label=\"ColumnRef\\nv.vorlnr\"];"
   "node7 -> node19 [label=\"right\"];"
   "node19 [label=\"BaseTable\\n table: professoren\\nschema: \\ncatalog: \\nalias: p\"];"
   "node7 -> node20 [label=\"ON\"];"
   "node20 [label=\"σ=\"];"
   "node20 -> node21 [label=\"left\"];"
   "node21 [label=\"ColumnRef\\np.persnr\"];"
   "node20 -> node22 [label=\"right\"];"
   "node22 [label=\"ColumnRef\\nv.gelesenvon\"];"
   "node0 -> node23 [label=\"WHERE\"];"
   "node23 [label=\"σAND\"];"
   "node23 -> node24 [label=\"child 1\"];"
   "node24 [label=\"σ>=\"];"
   "node24 -> node25 [label=\"left\"];"
   "node25 [label=\"ColumnRef\\ns.semester\"];"
   "node24 -> node26 [label=\"right\"];"
   "node26 [label=\"Constant\\n4\"];"
   "node23 -> node27 [label=\"child 2\"];"
   "node27 [label=\"σ>=\"];"
   "node27 -> node28 [label=\"left\"];"
   "node28 [label=\"ColumnRef\\ns.semester\"];"
   "node27 -> node29 [label=\"right\"];"
   "node29 [label=\"Constant\\n3\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected );
}


TEST_CASE("Parser:simpleGroupAggregation") {
   std::string sql{":select s.matrnr, avg(v.sws) "
         "from studenten as s "
         "join hoeren as h on h.matrnr=s.matrnr "
         "join vorlesungen as v on h.vorlnr=v.vorlnr "
         "where s.semester>=0 and s.semester>=1 "
         "group by s.matrnr "
         "having v.sws > 2"};
   driver drv{};
   drv.parse(sql);
   lingodb::ast::NodeIdGenerator idGen{};
   auto resultToDot = drv.result->toDotGraph(1, idGen);
   std::string expected{"node0 [label=\"SelectNode\"];"
   "node0 -> node1 [label=\"select list\"];"
   "node1 [label=\"Targets List\"];"
   "node1 -> node2 [label=\"target 1\"];"
   "node2 [label=\"ColumnRef\\ns.matrnr\"];"
   "node1 -> node3 [label=\"target 2\"];"
   "node3 [label=\"Functionname: avgagg\"];"
   "node3 -> node4 [label=\"arg 1\"];"
   "node4 [label=\"ColumnRef\\nv.sws\"];"
   "node0 -> node5 [label=\"FROM\"];"
   "node5 [label=\"JoinReftype: inner JOIN\\n\"];"
   "node5 -> node6 [label=\"left\"];"
   "node6 [label=\"JoinReftype: inner JOIN\\n\"];"
   "node6 -> node7 [label=\"left\"];"
   "node7 [label=\"BaseTable\\n table: studenten\\nschema: \\ncatalog: \\nalias: s\"];"
   "node6 -> node8 [label=\"right\"];"
   "node8 [label=\"BaseTable\\n table: hoeren\\nschema: \\ncatalog: \\nalias: h\"];"
   "node6 -> node9 [label=\"ON\"];"
   "node9 [label=\"σ=\"];"
   "node9 -> node10 [label=\"left\"];"
   "node10 [label=\"ColumnRef\\nh.matrnr\"];"
   "node9 -> node11 [label=\"right\"];"
   "node11 [label=\"ColumnRef\\ns.matrnr\"];"
   "node5 -> node12 [label=\"right\"];"
   "node12 [label=\"BaseTable\\n table: vorlesungen\\nschema: \\ncatalog: \\nalias: v\"];"
   "node5 -> node13 [label=\"ON\"];"
   "node13 [label=\"σ=\"];"
   "node13 -> node14 [label=\"left\"];"
   "node14 [label=\"ColumnRef\\nh.vorlnr\"];"
   "node13 -> node15 [label=\"right\"];"
   "node15 [label=\"ColumnRef\\nv.vorlnr\"];"
   "node0 -> node16 [label=\"WHERE\"];"
   "node16 [label=\"σAND\"];"
   "node16 -> node17 [label=\"child 1\"];"
   "node17 [label=\"σ>=\"];"
   "node17 -> node18 [label=\"left\"];"
   "node18 [label=\"ColumnRef\\ns.semester\"];"
   "node17 -> node19 [label=\"right\"];"
   "node19 [label=\"Constant\\n0\"];"
   "node16 -> node20 [label=\"child 2\"];"
   "node20 [label=\"σ>=\"];"
   "node20 -> node21 [label=\"left\"];"
   "node21 [label=\"ColumnRef\\ns.semester\"];"
   "node20 -> node22 [label=\"right\"];"
   "node22 [label=\"Constant\\n1\"];"
   "node0 -> node23 [label=\"GROUP BY\"];"
   "node23 [label=\"GROUP BY\"];"
   "node23 -> node24 [label=\"expr 1\"];"
   "node24 [label=\"ColumnRef\\ns.matrnr\"];"
   "node0 -> node25 [label=\"HAVING\"];"
   "node25 [label=\"σ>\"];"
   "node25 -> node26 [label=\"left\"];"
   "node26 [label=\"ColumnRef\\nv.sws\"];"
   "node25 -> node27 [label=\"right\"];"
   "node27 [label=\"Constant\\n2\"];"};

   std::erase(resultToDot, '\n');
   REQUIRE(resultToDot == expected );
}


} // namespace