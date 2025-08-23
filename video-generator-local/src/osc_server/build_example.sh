#!/bin/bash

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$ROOT/osc_server/build"

clang++ -std=c++20 -fPIC \
    -I"$ROOT/includes" \
    $(pkg-config --cflags liblo) \
    $(pkg-config --libs liblo) \
    "$ROOT/osc_server/cpp_example.cpp" \
    -o "$OUT/cpp_example"

echo "Build: $OUT_DIR/cpp_example"
