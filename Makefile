CFLAGS := -Wall -O2 -static -s -static-libgcc -static-libstdc++ -fPIE

SRC := hid-tester.cpp
TARGET := hid-tester

all: build cross

build:
	g++ $(CFLAGS) -o $(TARGET) $(SRC)

clean:
	-rm hid-tester hid-tester-*

keys:
	python mkkeys.py

cross:
	-aarch64-linux-gnu-g++ $(CFLAGS) -o $(TARGET)-aarch64 $(SRC)
	-arm-none-linux-gnueabihf-g++ $(CFLAGS) -o $(TARGET)-arm $(SRC)