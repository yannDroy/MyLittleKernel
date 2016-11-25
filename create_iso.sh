#!/bin/bash

cp kernel.bin ../mlk_0.1/isofiles/boot/
grub-mkrescue -o ../mlk_0.1/MyLittleKernel.iso ../mlk_0.1/isofiles/
