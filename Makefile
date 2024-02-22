SRC_DIR=./src
INC_DIR=./include
GATHER_DATA_SRC_DIR=$(SRC_DIR)/gatherDataDaemon

CC=gcc
CFLAGS=-I $(INC_DIR)
SNMP_FALGS=-I. `net-snmp-config --cflags` `net-snmp-config --agent-libs`
CLI_FLAGS=-lcli

GATHER_DATA_SRC=$(GATHER_DATA_SRC_DIR)/gatherDataNet.c			\
				$(GATHER_DATA_SRC_DIR)/gatherDataBase.c			\
				./test.c
GATHER_DATA_OBJ=$(GATHER_DATA_SRC:.c=.o)

all: gather_data

gather_data: $(GATHER_DATA_OBJ)
	$(CC) $(GATHER_DATA_OBJ) -o test

clean:
	rm -rf $(GATHER_DATA_OBJ)