#!/bin/bash

debug=1
source=.prog.s

for arg in $@; do
	if [ $arg = "-q" ] || [ $arg = "--quiet" ]; then
		debug=0
	elif [ $arg = "-d" ] || [ $arg = "--debug" ]; then
		debug=1
	fi
done

edit $source
riscv32-elf-as -o prog.o $source
if [ $? -ne 0 ]; then exit 1; fi
riscv32-elf-ld -T linker.ld -o prog.elf prog.o

if [ $debug -eq 1 ]; then
	riscv32-elf-objdump -d prog.elf
fi

riscv32-elf-objcopy -O binary prog.elf prog
rm prog.o prog.elf
