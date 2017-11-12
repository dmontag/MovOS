#!/bin/sh

docker cp ../. $CONTAINER:/root
docker exec $CONTAINER sh -c "cd /root && ./build.sh"
docker cp $CONTAINER:/root/os.bin .

cat os.bin nops.bin > os.dsk
dd if=os.dsk of=os-floppy.dsk bs=1474560 count=1

