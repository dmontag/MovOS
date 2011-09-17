#!/bin/sh

max=1228800
size=`wc -c os.bin | cut -d' ' -f1`
x=`echo "$max-$size" | bc`;
dd if=/dev/zero bs=$x count=1 >> os.bin
genisoimage -o os.iso -b os.bin os.bin
