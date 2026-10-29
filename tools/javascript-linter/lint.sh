#!/bin/bash
# Lint src/javascript without moving it.
# Usage: lint.sh [eslint options...]   (e.g. lint.sh --fix)

set -euo pipefail

LINTER_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT_DIR="$(cd "$LINTER_DIR/../.." && pwd)"

if [ ! -d "$LINTER_DIR/node_modules" ]; then
  (cd "$LINTER_DIR" && yarn install --frozen-lockfile)
fi

# With -c, eslint resolves file patterns from the current directory
cd "$ROOT_DIR"
"$LINTER_DIR/node_modules/.bin/eslint" -c "$LINTER_DIR/eslint.config.mjs" "$@" src/javascript
