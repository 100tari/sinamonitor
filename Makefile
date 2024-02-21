SRC_DIR=./src
INC_DIR=./include

CC=gcc
CFLAGS=-I $(INC_DIR)
SNMP_FALGS=-I. `net-snmp-config --cflags` `net-snmp-config --agent-libs`
CLI_FLAGS=-lcli