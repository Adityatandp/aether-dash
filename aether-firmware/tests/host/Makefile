CC ?= gcc
CFLAGS ?= -std=c11 -Wall -Wextra -O0 -g
ROOT := $(abspath ../..)

INCLUDES := \
	-I$(ROOT)/tests/host/include \
	-I$(ROOT)/components/aether_protocol/include \
	-I$(ROOT)/components/aether_hal/include \
	-I$(ROOT)/boards/dash-s35

SRCS := \
	test_protocol.c \
	$(ROOT)/components/aether_protocol/aether_protocol.c \
	$(ROOT)/components/aether_hal/src/aether_board.c

.PHONY: test clean

test: test_protocol
	./test_protocol

test_protocol: $(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -DAETHER_BOARD_ID=\"dash-s35\" $^ -o $@ -lm

clean:
	rm -f test_protocol
