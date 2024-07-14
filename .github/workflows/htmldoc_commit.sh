#!/bin/bash

set -eu

# Check if the pwd is the root of the repository
[ -d .git ]

CODELIB_PATH=`pwd`
DOCS_WORKDIR=/tmp/docs

# Commit the generated documentation

rm -rf ./docs ||:
mv $DOCS_WORKDIR ./
rm $(find ./docs -name .gitignore) ||:
git add docs
git commit -m 'update htmldoc by Github Actions' ||:
