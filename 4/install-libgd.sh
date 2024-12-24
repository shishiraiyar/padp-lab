#! /usr/bin/bash
curl -LO https://github.com/libgd/libgd/releases/download/gd-2.3.3/libgd-2.3.3.tar.gz
tar -xzf libgd-2.3.3.tar.gz
cd libgd-2.3.3/
./configure
make
sudo make install
