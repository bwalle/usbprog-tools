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
    echo "   *** compiler=$1 $2 $3 $4 ***"
    echo "---------------------------------------------------------"
    default
    echo ""

    if [ "$1" = "gcc" ] ; then
        C_COMPILER=gcc
        CXX_COMPILER=g++
    elif [ "$1" = "clang" ] ; then
        C_COMPILER=clang
        CXX_COMPILER=clang++
    else
        echo "Compiler '$1' not supported"
        exit 1
    fi

    cd "$BASEDIR"
    rm -fr build
    mkdir build
    cd build
    cmake \
        -DCMAKE_C_COMPILER="$C_COMPILER" \
        -DCMAKE_CXX_COMPILER="$CXX_COMPILER" \
        "-D$2" "-D$3" "-D$4" ..
    if ! make ; then
        echo "   Variant FAILED: *** compiler=$1 $2 $3 $4 ***"
    fi
    cd ..
    rm -fr build
    return $ret
}

for compiler in gcc clang ; do
    test_variant $compiler BUILD_ONLY_CORE=ON USE_QT5=OFF BUILD_GUI=OFF
    test_variant $compiler BUILD_ONLY_CORE=OFF USE_QT5=OFF BUILD_GUI=OFF
    test_variant $compiler BUILD_ONLY_CORE=OFF USE_QT5=OFF BUILD_GUI=ON
    test_variant $compiler BUILD_ONLY_CORE=OFF USE_QT5=ON BUILD_GUI=OFF
    test_variant $compiler BUILD_ONLY_CORE=OFF USE_QT5=ON BUILD_GUI=ON
done
