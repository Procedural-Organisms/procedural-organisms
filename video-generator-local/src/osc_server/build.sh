#!/bin/bash

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
OUT="$ROOT/osc_server/build"

clang++ -std=c++20 -fPIC \
    -I"$ROOT/includes" \
    $(pkg-config --cflags liblo) \
    $(pkg-config --libs liblo) \
    "$ROOT/osc_server/osc_server.cpp" \
    -o "$OUT/osc_server"
