#!/usr/bin/env bash
set -euo pipefail
TMPDIR=`mktemp --directory`

echo $TMPDIR
cp  resources/sql/tpch/initialize.sql $TMPDIR/initialize.sql

pushd $TMPDIR
cp -r /home/mor/projects/tpch-dbgen $TMPDIR
mv tpch-dbgen/* .
ls
make
set -x
./dbgen -f -s 1
ls -la .
chmod +r *.tbl
for table in ./*.tbl; do  sed -i 's/|$//' "$table"; done

"/home/mor/projects/lingo-db/build/lingodb-release/sql" /home/mor/projects/lingo-db/resources/data/tpch < initialize.sql
popd
