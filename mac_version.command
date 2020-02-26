#!/usr/bin/env bash

BASEDIR=$(dirname "$0")
cd $BASEDIR
chmod +x *.dylib
cd Release
chmod +x *
./fusion
