CFLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -O2
LDLIBS = -lgmp

A_TARGET = build/librebeltls.a
SO_TARGET = $(patsubst %.a,%.so,$(A_TARGET))
BIN_TARGET = bin/rebeltls
TARGETS = $(A_TARGET) $(SO_TARGET) $(BIN_TARGET)

SRCS = $(wildcard src/**/*.c src/*.c)
OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

MAIN_OBJ = src/main.o
LIB_OBJS = $(filter-out $(MAIN_OBJ),$(OBJS))

TESTS = $(patsubst %.c,%,$(wildcard test/*.c))

all: $(TARGETS) test

dev: CFLAGS += -g
dev: all

$(TESTS): $(A_TARGET)

test: CFLAGS += $(A_TARGET) -Isrc
test: $(TESTS)
	sh ./test/runtests.sh

# release: PREFIX = $(HOME)/.local
# release: CFLAGS += -O3
# release: $(TARGET)

# install: PREFIX = $(HOME)/.local
# install: release
# 	install -D $(TARGET) $(PREFIX)/$(TARGET)
# 	install -m 644 -D $(STDLIB) $(PREFIX)/$(STDLIB)

$(A_TARGET): build
$(A_TARGET): $(LIB_OBJS)
	$(AR) -rcs $@ $(LIB_OBJS)

$(SO_TARGET): build
$(SO_TARGET): $(LIB_OBJS)
	$(CC) $(LDLIBS) -shared -o $@ $(LIB_OBJS)

$(BIN_TARGET): $(MAIN_OBJ) $(A_TARGET) bin
	$(CC) $(LDLIBS) -o $@ $(MAIN_OBJ) $(A_TARGET)

build:
	@mkdir -p build

bin:
	@mkdir -p bin

-include $(DEPS)

# the black magic after the first line constructs the dep files correctly
%.d: %.c
	@$(CC) $(CFLAGS) -MM $*.c >$*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' <$*.d.tmp >$*.d
	@sed -e 's/.*://' -e 's/\\$$//' <$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >>$*.d
	@$(RM) $*.d.tmp

clean:
	$(RM) $(TARGETS) $(TESTS) || true
	$(RM) $(OBJS) $(DEPS) || true

.PHONY: dev test release install clean
