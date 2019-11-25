#!/bin/bash

VERSION=0.0.1
NAME=tea-time

tar -czvf ${NAME}-${VERSION}.tar.gz \
    --exclude=${NAME}-${VERSION}.tar.gz \
    --exclude=.git \
    --exclude=build \
    .
