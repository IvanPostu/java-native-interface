#!/usr/bin/env bash
# build-jar.sh
# Usage:
#   ./build-jar.sh <MainClass> <source_dir> <output_dir>
# Example:
#   ./build-jar.sh com.abc.TestExample ./src ./build

set -euo pipefail

if [ "$#" -ne 3 ]; then
  echo "Usage: $0 <MainClass> <source_dir> <output_dir>"
  exit 1
fi

MAIN_CLASS="$1"
SRC_DIR="$2"
OUT_DIR="$3"

[ -d "$SRC_DIR" ] || { echo "Error: source directory '$SRC_DIR' not found."; exit 1; }
mkdir -p "$OUT_DIR"

BUILD_DIR="$(mktemp -d)"
trap 'rm -rf "$BUILD_DIR"' EXIT

# Name jar after the Main class' simple name (e.g., TestExample.jar)
JAR_BASENAME="${MAIN_CLASS##*.}"
JAR_PATH="$OUT_DIR/${JAR_BASENAME}.jar"

echo "🛠️  Compiling Java sources from '$SRC_DIR' ..."
# Collect sources
mapfile -d '' SOURCES < <(find "$SRC_DIR" -type f -name '*.java' -print0)
if [ "${#SOURCES[@]}" -eq 0 ]; then
  echo "Error: no .java files found in '$SRC_DIR'."
  exit 1
fi

# Compile (classpath includes source tree so package imports resolve)
javac -cp "$SRC_DIR" -d "$BUILD_DIR" "${SOURCES[@]}"

# Verify Main class compiled (best-effort)
MAIN_CLASS_PATH="${MAIN_CLASS//.//}.class"
if [ ! -f "$BUILD_DIR/$MAIN_CLASS_PATH" ]; then
  echo "Error: Main class '$MAIN_CLASS' not found after compilation (looked for '$MAIN_CLASS_PATH')."
  exit 1
fi

# Copy non-source resources into the jar payload (preserve structure)
echo "📁 Copying non-source files ..."
# If there are no non-source files, rsync exits 23; ignore that with '|| true'
rsync -a --exclude='*.java' "$SRC_DIR"/ "$BUILD_DIR"/ || true

# Create executable JAR with Main-Class
echo "📦 Creating JAR: $JAR_PATH ..."
jar cfe "$JAR_PATH" "$MAIN_CLASS" -C "$BUILD_DIR" .

echo "✅ Build complete: $JAR_PATH"
echo "▶ Run: java -jar \"$JAR_PATH\""
