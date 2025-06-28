#!/usr/bin/env bash

set -e
BUILD_DIR=build

if [ ! -d "$BUILD_DIR" ];  then
  echo "Creating build directory: $BUILD_DIR/"
  mkdir "$BUILD_DIR"
fi

echo "Cleaning $BUILD_DIR"
rm -rf "$BUILD_DIR"/*

echo "Running CMake..."
cd "$BUILD_DIR"
cmake ..

echo "Running make"
make

echo "Launcing Game"

./MyGame
