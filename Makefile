.PHONY: all

CFLAGS = -Iinclude $(shell $(PKG_CONFIG) --cflags openssl)
LDFLAGS = $(shell $(PKG_CONFIG) --libs openssl)

SRCS = $(shell find . -name '*.c')

OBJS = $(SRCS:%.c=%.o)

all: cortex

cortex: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: cortex
	install -m0755 -D cortex $(PREFIX)/bin/cortex
