CC       = gcc
CXX      = g++
CFLAGS   = -g
CXXFLAGS = $(CFLAGS)

LIBPATH = ./
LIBS = -Wl,-rpath=./:./HCNetSDKCom -lhcnetsdk 
SRC = ./test.cpp
TARGET = ./test1

all: 
	$(CXX) $(OPTI) $(CXXFLAGS) $(SRC) -o $(TARGET) -L$(LIBPATH) $(LIBS)  `pkg-config --cflags --libs opencv`

.PHONY: clean
clean:
	rm -f $(TARGET)
