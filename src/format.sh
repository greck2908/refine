#!/usr/bin/env bash

#uncrustify -c ../uncrustify-gnu.cfg --replace $1*.h $1*.c

clang-format -style=Google -i *.c *.h

