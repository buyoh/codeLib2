#!/bin/bash

set -euo pipefail

# Check if the pwd is the root of the repository
[ -d .git ]

CODELIB_PATH=$(pwd)

tools/codeLib2-tools/target/release/gather \
  --basepath "$CODELIB_PATH" \
  --output-path-article /tmp/codelib_full.json \
  --output-path-collection /tmp/codelib_collection.json

ruby tools/codeLib2-tools/tools/refactor_code.rb \
  --basedir "$CODELIB_PATH" \
  --collection-path /tmp/codelib_collection.json \
  --inplace
