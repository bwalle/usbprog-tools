#!/bin/bash

# Tests all build variants on Linux

BASEDIR="$(pwd)"

highlight_on()
{
    echo -e "\033[36;1m"
}

default()
{
    echo -e "\033[0m"
}

test_variant()
{
    echo ""
    echo ""
    echo ""
    highlight_on
    echo "---------------------------------------------------------"
    echo "   *** $1 $2 $3 ***"
    echo "---------------------------------------------------------"
    default
    echo ""

    cd "$BASEDIR"
    rm -fr build
    mkdir build
    cd build
    cmake "-D$1" "-D$2" "-D$3" ..
    make
    cd ..
    rm -fr build
}

test_variant BUILD_ONLY_CORE=ON USE_QT5=OFF BUILD_GUI=OFF
test_variant BUILD_ONLY_CORE=OFF USE_QT5=OFF BUILD_GUI=OFF
test_variant BUILD_ONLY_CORE=OFF USE_QT5=OFF BUILD_GUI=ON
test_variant BUILD_ONLY_CORE=OFF USE_QT5=ON BUILD_GUI=OFF
test_variant BUILD_ONLY_CORE=OFF USE_QT5=ON BUILD_GUI=ON
