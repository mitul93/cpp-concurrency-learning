#!/usr/bin/env bash
set -euo pipefail

echo "(*) Running ./install_cmake.sh"

INSTALL_PREFIX="${1:?Usage: $0 <INSTALL_PREFIX>}"
INSTALL_PREFIX="${INSTALL_PREFIX%/}"
INSTALL_DIR="${INSTALL_PREFIX}"

CMAKE_VERSION="${CMAKE_VERSION:?CMAKE_VERSION environment variable is required}"

if [ "$(id -u)" -eq 0 ]; then
    echo "Do not run this script as root or with sudo"
    exit 1
fi

OS="$(uname -s)"

if [ "$OS" != "Linux" ]; then
    echo "Unsupported operating system: $OS (Linux only)"
    exit 1
fi

ARCH="$(uname -m)"

case "$ARCH" in
    x86_64)
        CMAKE_ARCH="x86_64"
        ;;
    aarch64|arm64)
        CMAKE_ARCH="aarch64"
        ;;
    *)
        echo "Unsupported architecture: $ARCH"
        exit 1
        ;;
esac

URL="https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-${CMAKE_ARCH}.sh"

TMP_INSTALLER="$(mktemp)"

echo "(*) Downloading CMake ${CMAKE_VERSION}"
echo "(*) URL: ${URL}"

curl -fsSL "${URL}" -o "${TMP_INSTALLER}"
chmod +x "${TMP_INSTALLER}"

echo "(*) Installing to ${INSTALL_DIR}"

mkdir -p $INSTALL_DIR

"${TMP_INSTALLER}" \
    --skip-license \
    --prefix="${INSTALL_DIR}"

rm -f "${TMP_INSTALLER}"

echo "(*) Installed CMake:" $(${INSTALL_DIR}/bin/cmake --version)

echo "(*) Add ${INSTALL_DIR}/bin to PATH"