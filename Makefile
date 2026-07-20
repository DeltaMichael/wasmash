# CC=emcc
# CFLAGS=-std=gnu99 -sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,callMain,HEAPU8 -sMODULARIZE=1 -sEXPORT_NAME=Foo -sENVIRONMENT=web -sASYNCIFY=0 -sINVOKE_RUN=0

CC=gcc
CFLAGS=-g -Wextra -Wall -Wundef -Wswitch-default -Wswitch-enum

IDIR=include
ODIR=obj
ARTEFACT=index

_DEPS=stack.h instruction.h list.h hashmap.h asm_lexer.h machine.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_TEST_DEPS=list.h
TEST_DEPS=$(patsubst %,$(IDIR)/%,$(_TEST_DEPS))

_OBJ=main.o stack.o instruction.o list.o hashmap.o asm_lexer.o machine.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

_TEST_OBJ=test.o list.o
TEST_OBJ=$(patsubst %,$(ODIR)/%,$(_TEST_OBJ))

_UNITTEST_OBJ=unittest.o list.o
UNITTEST_OBJ=$(patsubst %,$(ODIR)/%,$(_UNITTEST_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@ $(CFLAGS) -I$(IDIR)

mishmash: $(OBJ)
# $(CC) $^ -o $(ARTEFACT).html --shell-file shell.html $(CFLAGS)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

testmash: $(TEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

unitmash: $(UNITTEST_OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f $(OBJ) $(TEST_OBJ) $(ARTEFACT)* unitmash testmash mishmash

.PHONY: run

run:
	python -m http.server
