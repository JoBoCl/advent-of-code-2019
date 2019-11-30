#!/bin/sh
fswatch rect.h vec2.h vect_test.cc parser.h | xargs -I{} -n1 ./build_test.sh