#!/bin/bash

set -eu

# Check if the pwd is the root of the repository
[ -d .git ]

CODELIB_PATH=`pwd`
DOCS_WORKDIR=/tmp/docs

# Generate documentation into /tmp/docs

mkdir -p $DOCS_WORKDIR
tools/codeLib2-tools/target/release/gather \
  --basepath $CODELIB_PATH \
  --output-path-article tools/codeLib2-htmldoc/public/data/codelib_full.json

pushd tools/codeLib2-htmldoc
yarn && yarn build
ruby tools/generate_release_directory.rb --output-path $DOCS_WORKDIR
popd
