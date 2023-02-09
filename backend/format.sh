#!/bin/bash

find . | grep ".*\\.[c|h]\$" | xargs astyle --style=kr
find . | grep ".*\\.[c|h]pp\$" | xargs astyle --style=kr
astyle --style=kr *.cpp
cmake-format ./CMakeLists.txt > ./CMakeLists.txt.orig && cp ./CMakeLists.txt.orig ./CMakeLists.txt
