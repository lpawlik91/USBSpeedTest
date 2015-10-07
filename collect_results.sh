#!/bin/bash

BUFFER=20
MODE=S
SIZE_SET=10737420
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

BUFFER=20
MODE=A
SIZE_SET=10737420
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

BUFFER=64
MODE=S
SIZE_SET=107374
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

BUFFER=64
MODE=A
SIZE_SET=107374
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

BUFFER=64
MODE=S
SIZE_SET=10737
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

BUFFER=64
MODE=A
SIZE_SET=10737
FILENAME="$MODE"
FILENAME+="_"
FILENAME+="$SIZE_SET"
FILENAME+=".data"

while [ $BUFFER -gt 1 ]; do
	./libusbtest2/libusbtest2/libusb $MODE $BUFFER $SIZE_SET 1 >> Results/$FILENAME
	let BUFFER=BUFFER-2
done

