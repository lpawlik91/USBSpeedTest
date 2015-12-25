#!/bin/bash
cd ..
./prepare_files.sh
cd UT
make clean
make
./libusb_gt
