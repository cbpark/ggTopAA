SRCDIR		:= src
BINDIR		:= bin
CXX			:= c++
CXXFLAGS	:= -g -O2 -Wall -Wextra -std=c++14 -I$(SRCDIR)
MKDIR		:= mkdir -p
RM			:= rm
UNAME       := $(shell uname -s)

# ROOT
# CXXFLAGS += $(shell root-config --cflags)
# LDFLAGS   = $(shell root-config --ldflags)
# LIBS      = $(shell root-config --libs)

.PHONY: all clean

all: $(BINDIR)/main

$(BINDIR)/main: $(SRCDIR)/main.cc
	$(MKDIR) $(BINDIR)
	$(CXX) -o $@ $(CXXFLAGS) $<

clean:
	$(RM) $(BINDIR)/main
	$(RM) -r `find . -name "*.dSYM" -print`
