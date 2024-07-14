#!/bin/bash

cd $(dirname $0)
cd ../../

CODELIB_PATH=`pwd`
DOCS_WORKDIR=/tmp/docs

# Commit the generated documentation

rm -rf ./docs
mv $DOCS_WORKDIR ./
rm `find ./docs -name .gitignore`
git add docs
git commit -m 'update htmldoc by Github Actions' ||:
