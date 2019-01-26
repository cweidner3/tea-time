#!/bin/bash

DIR=$(cd $(dirname $0) && pwd)
BUILDDIR="${DIR}/build"

# Make sure the build directory exists
mkdir -p "${BUILDDIR}"

# Enter the build directory
cd "${BUILDDIR}/"

# Run Cmake
cmake ..
if [[ $? -eq 0 ]]; then
    echo "Success"
else
    echo "Failure"
fi

