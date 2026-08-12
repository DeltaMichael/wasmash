#!/bin/sh
clang -g -fsanitize=address -fsanitize=undefined -lncurses ../list.c main.c -o debug
