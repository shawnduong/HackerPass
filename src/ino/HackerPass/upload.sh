#!/bin/sh

DEVICE="$1"

if [[ "$DEVICE" == "" ]]; then
	echo "Usage: $0 <DEVICE>"
	exit -1
fi

if ! [ -w "$DEVICE" ]; then
	echo "$DEVICE lacks write access. Changing mode now."
	sudo chmod a+rw "$DEVICE"
fi

arduino --upload HackerPass.ino
