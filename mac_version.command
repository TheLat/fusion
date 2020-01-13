#!/usr/bin/env bash

BASEDIR=$(dirname "$0")
cd $BASEDIR
cd Release
echo "$PWD"
./fusion
