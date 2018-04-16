CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic
CLIBS=-lgmp
SOURCES=main.c generate.c generate.h
EXE=kry
ZIPNAME=xkubis13.zip

all:
	@echo "-- COMPILE ------------------------------------"
	${CC} ${CFLAGS} ${SOURCES} ${CLIBS} -o ${EXE}

clean:
	rm -rf ${EXE} ${ZIPNAME}

run: clean all
	@echo "-- RUN ----------------------------------------"
	./${EXE} ${args}

zip:
	zip ${ZIPNAME} Makefile doc.pdf main.c