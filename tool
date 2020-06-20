#!/bin/bash

if [[ $# -eq 0 ]]; then
    echo "usage: ./tool <subcommand> ..."
    exit 1
fi

subcmd=$1
shift
ruby "./tools/$subcmd.rb" $@
