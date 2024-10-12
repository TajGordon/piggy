.PHONY = build run clean
CC=clang++
SRC=src/main.cpp
OUTNAME=main

ifeq ($(OS),Windows_NT)
	OUT=$(OUTNAME).exe
else
	OUT=$(OUTNAME)
endif

build: $(SRC)
	$(CC) $(SRC) -o $(OUT) -std=c++20

run: build
	./main

clean: build
	rm outdata/outputdata*.csv
	rm $(OUT)
