#!/usr/bin/env bash

# build-so.sh
# Usage: ./build-so.sh <source_path_without_ext> <destination_dir>
# Example: ./build-so.sh ./src/mylib ./build

set -e  # Exit immediately on error

# --- Validate args ---
if [ "$#" -ne 2 ]; then
    echo "Usage: $0 <source_path_without_ext> <destination_dir>"
    exit 1
fi

SRC_PATH="$1"
DEST_DIR="$2"

# Derive filenames
SRC_CPP="${SRC_PATH}.cpp"
SRC_H="${SRC_PATH}.h"
LIB_NAME=$(basename "$SRC_PATH")
OUT_SO="lib${LIB_NAME}.so"

# --- Check if source files exist ---
if [ ! -f "$SRC_CPP" ]; then
    echo "Error: Source file '$SRC_CPP' not found."
    exit 1
fi

# Header file is optional but check if exists for clarity
if [ ! -f "$SRC_H" ]; then
    echo "Warning: Header file '$SRC_H' not found (continuing anyway)."
fi

# --- Create destination folder ---
mkdir -p "$DEST_DIR"

# --- Compile shared object ---
echo "Compiling $SRC_CPP → $DEST_DIR/$OUT_SO ..."
g++ -fPIC -shared "$SRC_CPP" -o "$DEST_DIR/$OUT_SO"

echo "✅ Build complete: $DEST_DIR/$OUT_SO"
