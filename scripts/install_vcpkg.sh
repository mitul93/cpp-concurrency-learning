#!/usr/bin/env bash

set -euo pipefail

echo "(*) Running ./install_vcpkg.sh"

INSTALL_PREFIX=$1

if [ -z "${VCPKG_VERSION:-}" ]; then
    echo "VCPKG_VERSION is not set"
    exit 1
fi

if [ -z "${INSTALL_PREFIX:-}" ]; then
    echo "INSTALL_PREFIX is not set"
    exit 1
fi

if [ "$(id -u)" -eq 0 ]; then
    echo "Script must be run as non-root user." >&2
    exit 1
fi

INSTALL_PREFIX="${INSTALL_PREFIX%/}"
INSTALL_DIR="${INSTALL_PREFIX}/vcpkg"

echo "(*) INSTALL_DIR : ${INSTALL_DIR}"
echo "(*) VCPKG_VERSION: ${VCPKG_VERSION}"

VCPKG_URI=https://github.com/microsoft/vcpkg.git

mkdir -p ${INSTALL_DIR}
git -c advice.detachedHead=false clone --quiet ${VCPKG_URI} ${INSTALL_DIR} --branch=${VCPKG_VERSION} --filter=tree:0
${INSTALL_DIR}/bootstrap-vcpkg.sh -disableMetrics

echo "(*) Installed vcpkg-tools to: $INSTALL_DIR/vcpkg"
echo "(*) Version:" 
${INSTALL_DIR}/vcpkg version --disable-metrics
echo "(*) set environment variable VCPKG_ROOT to ${INSTALL_DIR}/vcpkg"