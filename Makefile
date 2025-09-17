CC=emcc
CFLAGS=-std=gnu99 -sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,callMain,HEAPU8 -sMODULARIZE=1 -sEXPORT_NAME=Foo -sENVIRONMENT=web -sASYNCIFY=0 -sINVOKE_RUN=0

IDIR=include
ODIR=obj
ARTEFACT=index

_DEPS=stack.h instruction.h list.h asm_lexer.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o stack.o instruction.o list.o asm_lexer.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@

mishmash: $(OBJ)
	$(CC) $^ -o $(ARTEFACT).html --shell-file shell.html $(CFLAGS)

.PHONY: clean

clean:
	rm $(OBJ) $(ARTEFACT)*
