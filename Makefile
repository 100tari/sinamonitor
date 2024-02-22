SRC_DIR=./src
INC_DIR=./include
GATHER_DATA_SRC_DIR=$(SRC_DIR)/gatherDataDaemon
CLI_SRC_DIR=$(SRC_DIR)/cliDaemon

CC=gcc
CFLAGS=-I $(INC_DIR)
SNMP_FALGS=-I. `net-snmp-config --cflags` `net-snmp-config --agent-libs`
CLI_FLAGS=-lcli

GATHER_DATA_SRC=$(GATHER_DATA_SRC_DIR)/gatherDataNet.c			\
				$(GATHER_DATA_SRC_DIR)/gatherDataBase.c			\
				$(GATHER_DATA_SRC_DIR)/gatherDataDaemon.c		\
				$(GATHER_DATA_SRC_DIR)/gatherDataIpcHandler.c	\
				./cli-agent.c
GATHER_DATA_OBJ=$(GATHER_DATA_SRC:.c=.o)

CLI_SRC=$(CLI_SRC_DIR)/cliDaemon.c 			\
		$(CLI_SRC_DIR)/cliIpcHandler.c 		\
		$(CLI_SRC_DIR)/cliCommandNet.c 
CLI_OBJ=$(CLI_SRC:.c=.o)

all: gather_data cli cli-agent

gather_data: $(GATHER_DATA_OBJ)
	$(CC) $(GATHER_DATA_OBJ) -o $@

cli: $(CLI_OBJ)
	$(CC) $(CLI_OBJ) -o $@ $(CLI_FLAGS)

cli-agent: $(CLI_OBJ) $(GATHER_DATA_OBJ) 
	$(CC) $(GATHER_DATA_OBJ) $(CLI_OBJ) -o $@ $(CLI_FLAGS)

clean:
	rm -rf $(GATHER_DATA_OBJ) $(CLI_OBJ)