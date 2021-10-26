#!/bin/bash

qemu-system-x86_64 \
	-bios bios.bin \
	-hda fat:rw:hda-contents \
	-net none \
	-debugcon file:debug.log -global isa-debugcon.iobase=0x402 -vga std \
	-serial pipe:/tmp/ovmf
