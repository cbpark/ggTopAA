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
EXESRC  := $(SRCDIR)/main.cc
EXEOBJ  := $(EXESRC:.cc=.o)
EXE     := $(patsubst $(SRCDIR)/%.o,$(BINDIR)/%,$(EXEOBJ))
LIBSRC  := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ  := $(LIBSRC:.cc=.o)
LIB     := $(LIBDIR)/libggTopAA.a
HEAD    := $(filter-out $(EXESRC:.cc=.h),$(wildcard $(SRCDIR)/*.h))

.PHONY: all build show clean

all: $(EXE)

$(EXE): build $(LIB) $(EXEOBJ)
	$(CXX) $(LDFLAGS) $(LIBS) $(LIB) -o $@ $(EXEOBJ)

$(LIB): CXXFLAGS += -fPIC
$(LIB): $(LIBOBJ)
	ar crs $@ $^
	ranlib $@

build:
	$(MKDIR) $(BINDIR)
	$(MKDIR) $(LIBDIR)

show:
	@echo -- Summary of the current environment and setting:
	@echo "UNAME    = $(UNAME)"
	@echo "CXX      = $(CXX)"
	@echo "CXXFLAGS = $(CXXFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "EXESRC   = $(EXESRC)"
	@echo "EXEOBJ   = $(EXEOBJ)"
	@echo "EXE      = $(EXE)"
	@echo "LIBSRC   = $(LIBSRC)"
	@echo "LIBOBJ   = $(LIBOBJ)"
	@echo "LIB      = $(LIB)"
	@echo "HEAD     = $(HEAD)"

clean:
	$(RM) $(EXE) $(EXEOBJ)
	$(RM) $(LIB) $(LIBOBJ)
	$(RM) -r `find . -name "*.dSYM" -print`
