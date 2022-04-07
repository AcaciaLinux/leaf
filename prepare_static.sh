#!/bin/bash

cd leafcore

git clone --depth=1 https://github.com/curl/curl
git clone --depth=1 https://github.com/libarchive/libarchive

cd ..

git apply --directory=leafcore/curl patches/patch_curl.patch
git apply --directory=leafcore/libarchive patches/patch_libarchive.patch

echo "DONE!"
