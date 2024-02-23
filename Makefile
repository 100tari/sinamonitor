SRC_DIR=./src
INC_DIR=./include
GATHER_DATA_SRC_DIR=$(SRC_DIR)/gatherDataDaemon
CLI_SRC_DIR=$(SRC_DIR)/cliDaemon
SNMP_SRC_DIR=$(SRC_DIR)/snmpDaemon

CC=gcc
CFLAGS=-I $(INC_DIR)
SNMP_FALGS=-I. `net-snmp-config --cflags` `net-snmp-config --agent-libs`
CLI_FLAGS=-lcli

GATHER_DATA_SRC=$(GATHER_DATA_SRC_DIR)/gatherDataNet.c			\
				$(GATHER_DATA_SRC_DIR)/gatherDataBase.c			\
				$(GATHER_DATA_SRC_DIR)/gatherDataDaemon.c		\
				$(GATHER_DATA_SRC_DIR)/gatherDataIpcHandler.c	
GATHER_DATA_OBJ=$(GATHER_DATA_SRC:.c=.o)

CLI_SRC=$(CLI_SRC_DIR)/cliDaemon.c 			\
		$(CLI_SRC_DIR)/cliIpcHandler.c 		\
		$(CLI_SRC_DIR)/cliCommandNet.c 		\
		./cli-agent.c
CLI_OBJ=$(CLI_SRC:.c=.o)

SNMP_SRC=$(SNMP_SRC_DIR)/snmpDaemon.c		\
		 $(SNMP_SRC_DIR)/snmpIpcHandler.c 	\
		 $(SNMP_SRC_DIR)/snmpModuleNet.c   	\
		 ./snmp-agent.c 
SNMP_OBJ=$(SNMP_SRC:.c=.o)		

all: cli-agent snmp-agent

cli-agent: $(CLI_OBJ) $(GATHER_DATA_OBJ) 
	$(CC) $(GATHER_DATA_OBJ) $(CLI_OBJ) -o $@ $(CLI_FLAGS)

snmp-agent: $(SNMP_OBJ) $(GATHER_DATA_OBJ)
	$(CC) $(GATHER_DATA_OBJ) $(SNMP_OBJ) -o $@ $(SNMP_FALGS)

clean:
	rm -rf $(GATHER_DATA_OBJ) $(CLI_OBJ) $(SNMP_OBJ)