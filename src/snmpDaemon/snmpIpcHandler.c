#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>

#include "snmpDaemon/snmpIpcHandler.h"
#include "common/commonIpcHandler.h"

static int snmp_ipc_socket;
static int is_ipc_socket_init;

int
init_snmp_ipc_handler()
{
    if(is_ipc_socket_init)
        {snmp_log(LOG_WARNING, "[snmpIpcHandler] - Ipc Socket Has Been Initialized\n"); return -1;}

    snmp_log(LOG_DEBUG, "[snmpIpcHandler] - Start Init Ipc Handler\n");
    struct sockaddr_un  addr;
    struct timeval to;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, IPC_SOCK_PTH, sizeof(addr.sun_path) - 1);

    to.tv_sec = IPC_SOCK_RECV_TIMEOUT;
    to.tv_usec = 0;

    if((snmp_ipc_socket = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
        {snmp_log(LOG_WARNING, "[snmpIpcHandler] - Failed To Create Ipc Socket\n"); return -1;}
    snmp_log(LOG_DEBUG ,"[snmpIpcHandler] - Socket Init Successfull\n");

    if(setsockopt(snmp_ipc_socket, SOL_SOCKET, SO_RCVTIMEO, (void*) &to, sizeof(to)) < 0)
        {snmp_log(LOG_WARNING ,"[snmpIpcHandler] - Failed To Set Recv Timeout Option To Socket\n"); return -1;}
    snmp_log(LOG_DEBUG ,"[snmpIpcHandler] - Socket Set Option Successfull: Recv Time Out Set To %lds\n",to.tv_sec);  

    while(connect(snmp_ipc_socket, (struct sockaddr*) &addr, sizeof(addr)) < 0)
        {snmp_log(LOG_WARNING, "[snmpIpcHandler] - Failed To Connect To Server; Trying Again ...\n"); sleep(1);}
    snmp_log(LOG_DEBUG, "[snmpIpcHandler] - Connected To Server Successfully\n");

    is_ipc_socket_init = 1;
    return snmp_ipc_socket;
}

int
handle_cli_ipc(const char* req, char* data, size_t data_size)
{
    if(!is_ipc_socket_init)
        {snmp_log(LOG_WARNING, "[snmpIpcHandler] - Ipc Socket Is Not Initialized\n"); return -1;}

    if(send(snmp_ipc_socket,(void*) req, MAX_REQ_LEN, 0) < 0)
        {snmp_log(LOG_WARNING ,"[snmpIpcHandler] - Failed To Sending Request (%s) To Server\n", req); return -1;}
    snmp_info(LOG_INFO ,"[snmpIpcHandler] - Request %s Sent To Peer\n", req);
        
    if(recv(snmp_ipc_socket, (void*) data, data_size, 0) < 0)
        {snmp_log(LOG_WARNING ,"[snmpIpcHandler] - Failed To Receive Response From Server For Request (%s)\n", req); return -1;}
    snmp_log(LOG_INFO, "[snmpIpcHandler] - Response %s Received From Server\n", data);   
    
    return 1;
}




