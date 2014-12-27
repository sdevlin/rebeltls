CFLAGS = -Wall -Wextra -Werror -Wno-gnu-zero-variadic-macro-arguments -pedantic -std=c99 -O2

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

test: CFLAGS += -Isrc
test: $(TESTS)
	sh ./test/runtests.sh

$(A_TARGET): $(LIB_OBJS)
	@mkdir -p build
	$(AR) -rcs $@ $(LIB_OBJS)

$(SO_TARGET): $(LIB_OBJS)
	@mkdir -p build
	$(CC) $(LDFLAGS) $(LDLIBS) -shared -o $@ $(LIB_OBJS)

$(BIN_TARGET): $(MAIN_OBJ) $(A_TARGET)
	@mkdir -p bin
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $(MAIN_OBJ) $(A_TARGET)

-include $(DEPS)

# the black magic after the first line constructs the dep files correctly
%.d: %.c
	@$(CC) $(CFLAGS) -MM $*.c >$*.d
	@mv -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' <$*.d.tmp >$*.d
	@sed -e 's/.*://' -e 's/\\$$//' <$*.d.tmp | fmt -1 | sed -e 's/^ *//' -e 's/$$/:/' >>$*.d
	@$(RM) $*.d.tmp

tags:
	ctags -eR .

clean:
	$(RM) $(TARGETS) || true
	$(RM) $(TESTS) || true
	$(RM) $(OBJS) || true
	$(RM) $(DEPS) || true
	$(RM) *.log || true
	find . -name '*.dSYM' | xargs rm -rf

.PHONY: dev test release install tags clean
