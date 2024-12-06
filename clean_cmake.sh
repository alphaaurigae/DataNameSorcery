#!/bin/bash

BUILD_DIR=build
BIN_DIR=bin

clean() {
    echo "Cleaning build and bin directories..."
    rm -rf ${BUILD_DIR} ${BIN_DIR}
}


clean