#!/bin/sh

nasm -o boot.bin boot.asm
nasm -o kernboot.bin kernboot.asm

nasm -f elf -o kerninit.o kerninit.asm
nasm -f elf -o switch.o switch.asm
nasm -f elf -o lock.o   lock.asm
nasm -f elf -o pad.o    pad.asm

gcc -m32 -c -w -O0 -fno-pic -fno-stack-protector -fomit-frame-pointer *.c

ld -m elf_i386 -t --oformat binary -o kernel.bin -T kernel.ld

#./append.sh boot.bin kernel.bin os.bin
cat boot.bin kernboot.bin kernel.bin > os.bin
