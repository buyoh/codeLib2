#!/bin/bash

if [[ $1 == "-u" ]]; then
    umount ./tmp
else
    mount -t tmpfs -o size=4m tmpfs ./tmp
fi
