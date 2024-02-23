#ifndef _SNMP_IPC_HANDLER_H_
#define _SNMP_IPC_HANDLER_H_

#include <sys/types.h>

int                         init_snmp_ipc_handler();
int                         handle_cli_ipc(const char* req, char* data, size_t data_size);

#endif // _SNMP_IPC_HANDLER_H_