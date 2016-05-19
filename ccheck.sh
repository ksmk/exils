#!/bin/bash
find . -type f -iname '*.o' -exec rm {} \;
find . -type f -name "*.cc" -exec g++ -std=gnu++14 -Wall -c {} +
#find . -type f -name "*.cpp" -exec g++ -std=gnu++14 -O3 -Wall {} +
#find . -type f -name "*.o" -exec g++ -std=gnu++14 -O3 -lGL -lGLEW -lglfw {} +