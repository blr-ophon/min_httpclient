CC := gcc
OPT := -O0
CFLAGS = -std=gnu99 -fPIC -g -Wall -Wextra -pedantic $(OPT)


INCLUDES := -I./src/client -I./src/http_hl
HEADERS := $(shell find ./ -name '*.h')

SRC_DIR := ./src
BUILD_DIR := ./build

CFILES := $(shell find ./src -name '*.c')
OBJECTS := $(CFILES:${SRC_DIR}/%.c=$(BUILD_DIR)/%.o)
EXEC := ./minhttp

RESPS_DIR := ./responses

#HTTP Handler Library
HHL_C := $(shell find ${SRC_DIR}/http_hl -name '*.c')
HHL_O := $(HHL_C:${SRC_DIR}/%.c=$(BUILD_DIR)/%.o)
HHL_LIB := ./hhl.so


all: ${EXEC} ${HHL_LIB}
	mkdir -p ${RESPS_DIR}

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} ${INCLUDES} $^ -o $@

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${HEADERS}
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

${HHL_LIB}: ${HHL_O}
	${CC} -shared -o $@ $^ ${CFLAGS}


clean:
	rm -rf ${BUILD_DIR} ${EXEC} ${HHL_LIB} ${RESPS_DIR}

testrun: ${EXEC}
	./$< www.example.com

debug: ${EXEC}
	cgdb --args ./$< www.example.com

