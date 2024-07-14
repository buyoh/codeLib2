#!/bin/sh

cd $(dirname $0)

TARGET_SRC_DIR=../../src

yarn

mkdir -p target_src
mv $TARGET_SRC_DIR/javascript ./target_src/
npx eslint --fix .
mv ./target_src/javascript $TARGET_SRC_DIR