CC := gcc
OPT := -O0
CFLAGS = -std=gnu99 -fPIC -g -Wall -Wextra -pedantic $(OPT)

SRC_DIR := ./src
BUILD_DIR := ./build

INCLUDES := -I./include
HEADERS := $(shell find ./ -name '*.h')
CFILES := $(shell find ./ -name '*.c')
OBJECTS := $(CFILES:${SRC_DIR}/%.c=$(BUILD_DIR)/%.o)

EXEC := ./minhttp

all: ${EXEC}

${EXEC}: ${OBJECTS}
	${CC} ${CFLAGS} ${INCLUDES} $^ -o $@

${BUILD_DIR}/%.o: ${SRC_DIR}/%.c ${HEADERS}
	mkdir -p $(dir $@)
	$(CC) ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -rf ${BUILD_DIR} ${EXEC}

testrun: ${EXEC}
	./$< www.example.com

debug: ${EXEC}
	cgdb --args ./$< www.example.com

