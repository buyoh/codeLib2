#!/bin/bash

set -euo pipefail

# Check if the pwd is the root of the repository
[ -d .git ]

DOCS_WORKDIR=/tmp/docs

# Commit the generated documentation

rm -rf ./docs
mv "$DOCS_WORKDIR" ./
git add docs
# Commit only if there are changes
git diff --cached --quiet || git commit -m 'update htmldoc by Github Actions'
