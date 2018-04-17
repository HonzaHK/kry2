CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic
CLIBS=-lgmp
SOURCES=main.c generate.c generate.h
EXE=kry
ZIPNAME=xkubis13.zip

SAMPLE_E=0x10001
SAMPLE_D=0x658bcfdb43cb0d146b54feac19663852ab084a93f71d036edf373729eecad87d3f7353c03b5ffb457ade0953741e45c56db92474d90f4123a5604459e1b0d32ebeee52754dd04bfaae18822bd9fac67f56f89ab60c4cafba8177177ea6c652c24bb657cd0f2ee9a565e75f7d814c91e1935495db1987a812ca5caca4304d7599
SAMPLE_N=0x82d10833e8e771492f3942bf2bdb50769d44ec59cd4fa80ec9a5f785870fbdf044ec4e23078399456ddaac2d5636d989b11e7b06931705e5aca84366b610fb9c433a01a445fa159631fab7a2c2559c0f76f5cbed45ffc532f0ac3b63cfd7fbbadbb572b44aa1e01b2166422b5f6e939906b6980ffbf2dfc5017f727b75d0cd1f

SAMPLE_M=0x123456789
SAMPLE_C=0x51d053d2b5964a9462f6e4dd29e3522e443206a3276cf00b1e17ac96093a313012ab62849d54a5671947fa461eb3f6277bef64e065ee15f9cc56655dc992d8aa8cae94a202bc599fec4b76b0d1dfdc773cfce53c5195113e9fe3044cfb6c0c32b4da447eaf27202991eb6cc496f27cc40602f0054adb31fdc95ea1e9b7ec1e56

all:
	@echo "-- COMPILE ------------------------------------"
	${CC} ${CFLAGS} ${SOURCES} ${CLIBS} -o ${EXE}

clean:
	rm -rf ${EXE} ${ZIPNAME}

run-generate: clean all
	@echo "-- RUN GENERATE -------------------------------"
	@./${EXE} -g 1024

run-encrypt: clean all
	@echo "-- RUN ENCRYPT --------------------------------"
	@./${EXE} -e ${SAMPLE_E} ${SAMPLE_N} ${SAMPLE_M}

run-decrypt: clean all
	@echo "-- RUN DECRYPT --------------------------------"
	@./${EXE} -d ${SAMPLE_D} ${SAMPLE_N} ${SAMPLE_C}

zip:
	zip ${ZIPNAME} Makefile doc.pdf main.c
