CEPHES_DIR  := cephes
CEPHES_SRCS := $(wildcard $(CEPHES_DIR)/*.c)
CEPHES_OBJS := $(CEPHES_SRCS:.c=.o)

LIB      += $(LIBDIR)/libmd.a
HEAD     += $(CEPHES_DIR)/cephes_hyp21f.h
CXXFLAGS += -I$(CEPHES_DIR)

$(LIBDIR)/libmd.a: CFLAGS += -g -O2 -fno-builtin -fPIC -I$(CEPHES_DIR)
$(LIBDIR)/libmd.a: $(CEPHES_OBJS)
	$(AR) $@ $^
	ranlib $@

clean::
	$(RM) $(CEPHES_OBJS)
