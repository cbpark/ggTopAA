SRCDIR		:= src
BINDIR		:= bin
CXX			:= c++
CXXFLAGS	:= -g -O2 -Wall -Wextra -std=c++14 -I$(SRCDIR)
MKDIR		:= mkdir -p
RM			:= rm -f

# ROOT
# CXXFLAGS += $(shell root-config --cflags)
# LDFLAGS   = $(shell root-config --ldflags)
# LIBS      = $(shell root-config --libs)

all: $(BINDIR)/main

$(BINDIR)/main: $(SRCDIR)/main.cc
	$(MKDIR) $(BINDIR)
	$(CXX) -o $@ $(CXXFLAGS) $<

.PHONY: clean
clean:
	$(RM) $(BINDIR)/main
	$(RM) -r `find . -name "*.dSYM" -print`
