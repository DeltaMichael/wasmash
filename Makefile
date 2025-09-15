CC=emcc
CFLAGS=-sEXPORTED_FUNCTIONS=_main -sEXPORTED_RUNTIME_METHODS=ccall,cwrap,callMain -sMODULARIZE=1 -sEXPORT_NAME=Foo -sENVIRONMENT=web -sASYNCIFY=0 -sINVOKE_RUN=0

IDIR=include
ODIR=obj
ARTEFACT=index

_DEPS=stack.h
DEPS=$(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ=main.o stack.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

$(ODIR)/%.o: %.c $(DEPS)
	$(CC) -c $< -o $@

mishmash: $(OBJ)
	$(CC) $^ -o $(ARTEFACT).html --shell-file shell.html $(CFLAGS)

.PHONY: clean

clean:
	rm $(OBJ) $(ARTEFACT)*
