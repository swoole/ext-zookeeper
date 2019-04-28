#!/usr/bin/env bash

CURRENT=`pwd`
circleci local execute run -e LOCAL=1 -v $CURRENT/data:/tmp/data