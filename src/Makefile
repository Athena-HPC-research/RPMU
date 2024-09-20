SHELL:=/bin/bash

FILENAME=main.c
OUTPUT=rpmu

debug:
	gcc $(FILENAME) RPMU.c buffer.c -o $(OUTPUT)

optimised:
	gcc $(FILENAME) RPMU.c buffer.c -O3 -o $(OUTPUT)