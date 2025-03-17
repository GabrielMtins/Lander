#!/bin/sh

if [ ! -d ./build ]; then
	cmake -B build
fi

if [ $? -eq 0 ]; then
	cd build && make -j16 && cd .. && ./build/Lander
fi
