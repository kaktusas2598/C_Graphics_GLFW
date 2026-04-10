#!/usr/bin/env bash

set -e

BUILD_DIR="build"

echo "== Configuring =="
cmake -S . -B $BUILD_DIR

echo "== Building =="
cmake --build $BUILD_DIR

echo "== Done =="
