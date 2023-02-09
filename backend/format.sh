#!/bin/bash

find . | grep -E "\\.[ch]p{2}?\$" | xargs -n 1 -P 0 astyle --style=kr
cmake-format ./CMakeLists.txt > ./CMakeLists.txt.orig && cp ./CMakeLists.txt.orig ./CMakeLists.txt
