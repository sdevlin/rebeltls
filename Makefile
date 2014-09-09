CFLAGS=-Wall -Wextra -Werror -pedantic -std=c99 -O2
LIBS=-lgmp $(OPTLIBS)

SOURCES=$(wildcard src/**/*.c src/*.c)
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

TEST_SRC=$(wildcard test/*_test.c)
TESTS=$(patsubst %.c,%,$(TEST_SRC))

BIN_TARGET=bin/tls
A_TARGET=build/libtls.a
SO_TARGET=$(patsubst %.a,%.so,$(A_TARGET))

all : $(BIN_TARGET) $(A_TARGET) $(SO_TARGET) test

dev : CFLAGS=-g -Isrc -Wall -Wextra -Werror -pedantic -std=c99 $(OPTFLAGS)
dev : all

$(BIN_TARGET) : build $(OBJECTS)
	$(CC) $(LIBS) -o $@ $(OBJECTS)

$(A_TARGET) : CFLAGS += -fPIC
$(A_TARGET) : build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET) : build $(OBJECTS)
	$(CC) $(LIBS) -shared -o $@ $(OBJECTS)

build :
	@mkdir -p build
	@mkdir -p bin

.PHONY : test
test : CFLAGS += $(A_TARGET)
test : $(TESTS)
	sh ./test/runtests.sh

clean :
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

install : all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(A_TARGET) $(DESTDIR)/$(PREFIX)/lib/
