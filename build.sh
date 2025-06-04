#!/usr/bin/env bash

set -e

# Initialize variables
CLEAN=false
SETUP=false
TARGET="DEBUG"
VENDOR_PKG=""

usage() {
    echo "Usage: $0 [OPTIONS] <vendor_pkg>"
    echo "Options:"
    echo "  -c, --clean           Clean build environment"
    echo "  -s, --setup           Setup and update environment"
    echo "  -t, --target TARGET   Set build target (default: DEBUG)"
    echo "Example:"
    echo "  $0 -s -t RELEASE Vendor/devicePkg"
    exit 1
}

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        -c|--clean)
            CLEAN=true
            shift
            ;;
        -s|--setup)
            SETUP=true
            shift
            ;;
        -t|--target)
            if [[ -n "$2" ]]; then
                TARGET="$2"
                shift 2
            else
                echo "Error: --target requires an argument"
                usage
            fi
            ;;
        -h|--help)
            usage
            ;;
        *)
            if [[ -z "$VENDOR_PKG" ]]; then
                VENDOR_PKG="$1"
                shift
            else
                echo "Error: Unknown argument '$1'"
                usage
            fi
            ;;
    esac
done

if [[ -z "$VENDOR_PKG" ]]; then
    echo "Error: vendor_pkg is required."
    usage
fi


PLATFORM_SCRIPT="Platform/${VENDOR_PKG}/PlatformBuild.py"

# Setup
if [ "$SETUP" = true ]; then
    python3 "$PLATFORM_SCRIPT" --setup -t "$TARGET"
    python3 "$PLATFORM_SCRIPT" --update -t "$TARGET"
    exit 0
fi

# Clean
if [ "$CLEAN" = true ]; then
    python3 "$PLATFORM_SCRIPT" --cleanonly
    find Common/edk2/BaseTools -type f \( -name "*.o" -o -name "*.d" \) -delete
    make -C Bootshim clean
    exit 0
fi

# Build BaseTools
make -C Common/edk2/BaseTools

# Main build
python3 "$PLATFORM_SCRIPT" "TARGET=$TARGET"
