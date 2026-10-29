#!/bin/bash

set -euo pipefail

# Check if the pwd is the root of the repository
[ -d .git ]

CODELIB_PATH=$PWD

tools/codeLib2-tools/target/release/gather \
  --basepath "$CODELIB_PATH" \
  --strict \
  --output-path-article /tmp/codelib_full.json \
  --output-path-collection /tmp/codelib_collection.json

mkdir -p /tmp/autotest
ruby tools/codeLib2-tools/tools/autotest.rb \
  --basedir "$CODELIB_PATH" \
  --collection-path /tmp/codelib_collection.json \
  --tempdir /tmp/autotest \
  --output-result-path /tmp/autotest.json

ruby tools/codeLib2-tools/tools/dump_result_to_markdown.rb \
  --result-path /tmp/autotest.json \
  > /tmp/autotest.md
