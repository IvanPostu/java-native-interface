#!/bin/bash
set -euo pipefail

# Usage: ./gen_headers.sh <SrcFolder> <OutputFolder>
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <SrcFolder> <OutputFolder>"
    exit 1
fi

SRC_DIR="$1"
OUT_DIR="$2"

# Validate source folder
if [ ! -d "$SRC_DIR" ]; then
    echo "Error: Source folder '$SRC_DIR' does not exist."
    exit 1
fi

# Create output folder if missing
mkdir -p "$OUT_DIR"

# Create temporary directory for compiled .class files
BUILD_DIR="$(mktemp -d)"

# Compile all .java files under SRC_DIR into BUILD_DIR, generating headers into OUT_DIR
echo "Compiling sources in '$SRC_DIR'..."
javac -d "$BUILD_DIR" -h "$OUT_DIR" $(find "$SRC_DIR" -type f -name "*.java")

echo "✅ Header files generated in: $OUT_DIR"
rm -rf "$BUILD_DIR"
echo "Done."
