#!/bin/bash

DIR=$(cd $(dirname $0) && pwd)
BUILDDIR="${DIR}-eclipse"

# Check if directory exists and if it is
# good to continue to modify it
if [[ -e "${BUILDDIR}" ]]; then
    if [[ ! -d "${BUILDDIR}" ]]; then
        (>&2 echo "${BUILDDIR} exists and is not a directory")
        exit 1
    fi
    read -p "${BUILDDIR} already exists, continue? [yN]" useropt
    if [[ "${useropt:-n}" != 'y' ]];then
        exit 1
    fi
else
    mkdir -p "${BUILDDIR}"
fi

# Enter the build directory
cd "${BUILDDIR}/"

# Run Cmake
cmake -G "Eclipse CDT4 - Unix Makefiles" \
    "${DIR}"
if [[ $? -eq 0 ]]; then
    echo "Success"
else
    echo "Failure"
fi

