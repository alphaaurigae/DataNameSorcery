#!/bin/bash

BUILD_DIR='build'
BIN_DIR='bin'

configure() {
    echo "Configuring project with cmake..."
    mkdir -p ${BUILD_DIR}
    cmake -S . -B ${BUILD_DIR} -DCMAKE_VERBOSE_MAKEFILE=ON
}

build() {
    echo "Building project..."
    cmake --build ${BUILD_DIR} --target all -- -j$(nproc) --debug
}

configure
build