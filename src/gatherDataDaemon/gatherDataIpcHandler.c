#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "gatherDataDaemon/gatherDataIpcHandler.h"
#include "gatherDataDaemon/gatherDataBase.h"
#include "common/commonIpcHandler.h"

#include "common/logger.h"

static int ipc_socket_fd; 
static int con_socket_fd; 

int
init_gather_data_ipc_handler()
{
    __LOG_INFO__("Start Init Ipc Handler\n");
    struct sockaddr_un addr;

    if((ipc_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        {__LOG_WARN__("Failed To Create Ipc Socket\n"); return -1;}
    __LOG_INFO__("Ipc Socket Init Successfull\n");

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, IPC_SOCK_PTH, sizeof(addr.sun_path) - 1);

    if(bind(ipc_socket_fd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        {__LOG_WARN__("Failed To Bind Socket\n"); return -1;}
    __LOG_INFO__("Socket Bind Successfull\n");

    if(listen(ipc_socket_fd, 100) < 0)
        {__LOG_WARN__("Failed To Listen Socket\n"); return -1;}
    __LOG_INFO__("Socket Listen Successfull\n");
    
    if((con_socket_fd = accept(ipc_socket_fd, NULL, 0)) < 0)
        {__LOG_WARN__("Failed To Accept New Connection\n"); return -1;}
    __LOG_INFO__("New Connection Accepted : %d\n", con_socket_fd);

    return ipc_socket_fd;
}

int
handle_gather_data_ipc()
{
    char    buf_recv[MAX_REQ_LEN] = {0};
    char    buf_send[MAX_RES_LEN] = {0};
    dataBase db;

    while(1)
    {
        if(recv(con_socket_fd,(void*) buf_recv, MAX_REQ_LEN, 0) > 0)
        {
            __LOG_INFO__("Recevied New Request From Connection (%d): %s\n", con_socket_fd, buf_recv);

            get_data_base(&db);
            if(strcmp(buf_recv, RX_TRAFFIC_REQ) == 0)
                sprintf(buf_send, "%lf", db.data_rxTraffic);
            if(strcmp(buf_recv, TX_TRAFFIC_REQ) == 0)
                sprintf(buf_send, "%lf", db.data_txTraffic);

            if(send(con_socket_fd, (void*) buf_send, MAX_RES_LEN, 0) < 0)
            {
                __LOG_WARN__("Failed To Send Response (%s) To Connection (%d) : %s\n", buf_send, con_socket_fd, strerror(errno));
                close(ipc_socket_fd);return -1;
            } __LOG_INFO__("Respond (%s) Sent To Connection (%d)\n", buf_send, con_socket_fd);
        } else 
        {
            __LOG_WARN__("Recv Time Out\n");
            close(ipc_socket_fd);return -1;
        }
    } return 1;
}

