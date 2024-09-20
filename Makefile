.PHONY: all

CFLAGS = -Iinclude $(shell $(PKG_CONFIG) --cflags openssl)
LDFLAGS = $(shell $(PKG_CONFIG) --libs openssl)

SRCS = $(shell find . -name '*.c')

OBJS = $(SRCS:%.c=%.o)

all: cloak

cloak: $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: cloak
	install -m0755 -D cloak $(PREFIX)/bin/cloak
