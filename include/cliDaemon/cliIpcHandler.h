#ifndef _CLI_IPC_HANDLER_H_
#define _CLI_IPC_HANDLER_H_

#include <sys/types.h>

int                 init_cli_ipc_handler();
int                 handle_cli_ipc(const char* req, char* data, size_t data_size);

#endif // _CLI_IPC_HANDLER_H_