#!/bin/bash

set -e

cd leafcore

echo "Cloning curl..."
git clone --depth=1 https://github.com/curl/curl

echo "Cloning libarchive..."
git clone --depth=1 https://github.com/libarchive/libarchive

cd ..

echo "Patching curl..."
git apply --directory=leafcore/curl patches/patch_curl.patch

echo "Patching libarchive..."
git apply --directory=leafcore/libarchive patches/patch_libarchive.patch

echo "DONE!"
