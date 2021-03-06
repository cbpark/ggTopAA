PKGNAME     := ggTopAA
VERSION     := 0.1.0
SRCDIR      := src
BINDIR      := bin
LIBDIR      := lib
CXXFLAGS    := -g -O3 -m64 -march=native -Wall -Wextra -pipe -I$(SRCDIR)
CFLAGS      := -g -O3 -m64 -march=native -Wall -Wextra -pipe
LDFLAGS     := -O3 -m64
LIBS        :=
AR          := ar crs
MKDIR       := mkdir -p
RM          := rm -f
UNAME       := $(shell uname -s)

ifeq ($(UNAME), Darwin)
	CXXFLAGS += -stdlib=libc++
endif

# Targets
EXE     := $(BINDIR)/gg2aa $(BINDIR)/gg2aa_contour $(BINDIR)/gg2aa_bestfits
EXESRC  := $(patsubst $(BINDIR)/%,$(SRCDIR)/%.cc,$(EXE))
EXEOBJ  := $(EXESRC:.cc=.o)
LIB     := $(LIBDIR)/lib$(PKGNAME).a
LIBSRC  := $(filter-out $(EXESRC),$(wildcard $(SRCDIR)/*.cc))
LIBOBJ  := $(LIBSRC:.cc=.o)
HEAD    := $(filter-out $(EXESRC:.cc=.h),$(wildcard $(SRCDIR)/*.h))

# ROOT (https://root.cern.ch)
CXXFLAGS += $(shell root-config --cflags)
## for the old ABI of GCC.
# ifeq ($(UNAME), Linux)
# 	CXXFLAGS += -D_GLIBCXX_USE_CXX11_ABI=0
# endif
CXXFLAGS += -std=c++14 -pedantic
LDFLAGS  += $(shell root-config --ldflags)
LIBS     += $(shell root-config --libs)
LIBS     += -lMathMore -lMinuit2

.PHONY: all build show clean

all: $(EXE)

# cephes (http://www.netlib.org/cephes/)
CEPHES_DIR := cephes
include $(CEPHES_DIR)/module.mk

$(BINDIR)/%: $(SRCDIR)/%.o build $(LIB)
	$(CXX) $(LDFLAGS) -o $@ $< $(LIB) $(LIBS)

$(LIBDIR)/lib$(PKGNAME).a: CXXFLAGS += -fPIC -I$(CEPHES_DIR)
$(LIBDIR)/lib$(PKGNAME).a: $(LIBOBJ)
	$(AR) $@ $^
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
	@echo "CC       = $(CC)"
	@echo "CFLAGS   = $(CFLAGS)"
	@echo "LDFLAGS  = $(LDFLAGS)"
	@echo "LIBS     = $(LIBS)"
	@echo "EXE      = $(EXE)"
	@echo "EXESRC   = $(EXESRC)"
	@echo "LIB      = $(LIB)"
	@echo "LIBSRC   = $(LIBSRC)"
	@echo "HEAD     = $(HEAD)"

clean::
	$(RM) $(LIBOBJ) $(EXEOBJ)
	$(RM) $(EXE) $(LIB)
	$(RM) -r $(BINDIR) $(LIBDIR)
