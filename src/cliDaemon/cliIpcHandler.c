#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include "cliDaemon/cliIpcHandler.h"
#include "common/commonIpcHandler.h"

#include "common/logger.h"

static int cli_ipc_socket;

int
init_cli_ipc_handler()
{
    __LOG_INFO__("Start Init Ipc Handler\n");
    struct sockaddr_un  addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, IPC_SOCK_PTH, sizeof(addr.sun_path) - 1);

    if((cli_ipc_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        {__LOG_WARN__("Failed To Create Ipc Socket\n"); return -1;}
    __LOG_INFO__("Ipc Socket Init Successfull\n");

    while(connect(cli_ipc_socket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        { __LOG_WARN__("Failed To Connect To Server; Trying Again ...\n"); sleep(1);}
    __LOG_INFO__("Connected To Server Successfully\n");

    return 1;
}

int
handle_cli_ipc(const char* req, char* data, size_t data_size)
{
    if(send(cli_ipc_socket,(void*) req, MAX_REQ_LEN, 0) < 0)
        {__LOG_WARN__("Failed To Sending Request (%s) To Server\n", req); return -1;}
    __LOG_INFO__("Request %s Sent To Peer\n", req);
        
    if(recv(cli_ipc_socket, (void*) data, data_size, 0) < 0)
        {__LOG_WARN__("Failed To Receive Response From Server For Request (%s)\n", req); return -1;}
    __LOG_INFO__("Response %s Received From Server\n", data);   
    
    return 1;
}