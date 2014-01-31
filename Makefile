SHELL = /bin/sh

.SUFFIXES:
.SUFFIXES: .cpp .o

OBJDIR := build
SRCDIR := src
BINDIR := bin

CXXFLAGS = -g -O -std=c++11

prefix = .
exec_prefix = $(prefix)
bindir = $(prefix)/bin
includedir = $(prefix)/include
libdir = $(exec_prefix)/lib
srcdir = ./src

objects = rope.o

all: $(bindir)/rope

$(addprefix $(libdir)/, %.o): %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(srcdir) -c $@ $^

lib/rope.o: src/rope.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(srcdir) -I$(includedir) -c $^

$(bindir)/rope: $(addprefix $(libdir)/,$(objects))
	@echo $(objects)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -I$(includedir) -I$(libdir) -c -o $@ $^

.PHONY: clean

clean:
	rm rope $(objects)