CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -pedantic
CLIBS=-lgmp
SOURCES=main.c kry_rsa.c kry_rsa.h
EXE=kry
ZIPNAME=xkubis13.zip

#== values with randseed set to 0: =====================
SAMPLE_P=0xdb8ce72430d9eefe1fc622b02906e9759f1b22dafeb504d4d313692bea3f61f9f8ad4f2777ec644e643bf50f97b0040747b13bd58cc1611320da962fbc488bbf
SAMPLE_Q=0xb3d5d1de714463ce51d684b6927be1969a9bf569d5007d626611d3ea4b3b8881076c71499e5345aee247230b2023fda8c4c07921f593c6bfccf3b58b83de91a5

#E hardcoded
SAMPLE_E=0x10001
SAMPLE_D=0x535f114df1ed4cb94b800ed0596efec72706a72780010348bf711ccd3d2943fb34b9863b8ccc8e524757764ac47a8941e47ec334829780b8ce7d6497c945dbdb03197518c042213c4e981ee7ac8c6c4d4076ca81026436be7a73b5aeb3714024c274b0a53e13d1bd003709c5ecd7146f0d90b2bd5a11dbbccd9a4ea8bbd20029
SAMPLE_N=0x9a3ae5d76d4dddc0a0e46b8e7e68bcacdcab88fc9e8b80a16c2705d935badaae32b7de5581ca0b1af902ada18baee8101ccb2ae9246d4fd7fd8f1b584f3d387fbfe7be1ee3d6d6c3ea6dbb261918e0f47c65f1f632e0f3feff869355634944fb05d58b881414cfcf4255ee477edf31d266cff3c6d521911d0731288a5e8b411b

SAMPLE_M=0x123456789
SAMPLE_C=0x4e33c67df3d74709dda261477c00d8cbab4b8a80d916191cec09fa5ae79e08a18393444ce164989ba2f45b130c5b6d0a036317cdc8cfd62afb24b11ad4d77e15a5baf2a59e08b7b1b03a5afb86223d57f039d1ce2699d2a3fe914f0070175afa6a181048c2b6178004e54bfb52370afe93b5e2cec799d25b2ee9899da2e691d6
#========================================================

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
	zip ${ZIPNAME} Makefile doc/doc.pdf main.c
