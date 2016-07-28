PKGNAME     := ggTopAA
VERSION     := 0.1.0
SRCDIR      := src
BINDIR      := bin
LIBDIR      := lib
CXX         := c++
CXXFLAGS    := -g -O2 -Wall -Wextra -std=c++14 -pedantic -I$(SRCDIR)
LDFLAGS     := -g
MKDIR       := mkdir -p
RM          := rm -f
UNAME       := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	CXXFLAGS += -stdlib=libc++
endif

# ROOT
# CXXFLAGS += $(shell root-config --cflags)
# LDFLAGS  += $(shell root-config --ldflags)
# LIBS      = $(shell root-config --libs)

# Targets
EXE     := $(BINDIR)/main
EXESRC  := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ  := $(EXESRC:.cc=.o)
LIB     := $(LIBDIR)/lib$(PKGNAME).a
LIBSRC  := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ  := $(LIBSRC:.cc=.o)
HEAD    := $(filter-out $(EXESRC:.cc=.h),$(wildcard $(SRCDIR)/*.h))

.PHONY: all build show clean

all: $(BINDIR)/main

$(BINDIR)/main: build $(LIB) $(SRCDIR)/main.o
	$(CXX) $(LDFLAGS) $(LIBS) $(LIB) -o $@ $(SRCDIR)/main.o

$(LIB): CXXFLAGS += -fPIC
$(LIB): $(LIBOBJ)
	ar crs $@ $^
	ranlib $@

build:
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(LIBDIR)

show:
	@echo -- Summary of the current environment and setting:
	@echo "PKGNAME  = $(PKGNAME)"
	@echo "VERSION  = $(VERSION)"
	@echo "UNAME    = $(UNAME)"
	@echo "CXX      = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "EXE      = $(EXE)"
	@echo "EXESRC   = $(EXESRC)"
	@echo "EXEOBJ   = $(EXEOBJ)"
	@echo "LIB      = $(LIB)"
	@echo "LIBSRC   = $(LIBSRC)"
	@echo "LIBOBJ   = $(LIBOBJ)"
	@echo "HEAD     = $(HEAD)"

clean:
	$(RM) $(EXE) $(EXEOBJ)
	$(RM) $(LIB) $(LIBOBJ)
	$(RM) -r `find . -name "*.dSYM" -print`
