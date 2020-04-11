#!/bin/bash
for i in `find source | awk /^.*\.c$/` `find include | awk /^.*\.h$/`
do
    if ! grep -q Copyright $i
    then
        cat LICENSE $i > $i.new && mv $i.new $i
    fi
done