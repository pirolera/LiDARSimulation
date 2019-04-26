#!/bin/bash

if [ ! -d json ]
then
  git clone https://github.com/nlohmann/json.git
fi

cd json/
if [ ! -d build ]
then
  mkdir build
fi

cd build/
cmake ../
sudo make -j2 install