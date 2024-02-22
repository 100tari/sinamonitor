#include <sys/socket.h>
#include <netinet/in.h>

#include "cliDaemon/cliDaemon.h"

#include "common/logger.h"

static int cli_socket;

int
init_cli_daemon_socket()
{
    __LOG_INFO__("Start Init Cli Socket\n");
    struct sockaddr_in saddr;
    int on = 1;

    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htons(INADDR_ANY);
    saddr.sin_port = htons(CLI_PORT);

    if((cli_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {__LOG_WARN__("Failed Init Cli Socket\n"); return -1;}
    __LOG_INFO__("Cli Socket Init Successfull\n");

    if(setsockopt(cli_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int)) < 0)
        {__LOG_WARN__("Failed To Set Reuse Addr To Socket\n"); return -1;}
    __LOG_INFO__("Cli Socket Set Option Successfull\n");


    if(bind(cli_socket ,(struct sockaddr*) &saddr, sizeof(saddr)) < 0) 
        {__LOG_WARN__("Failed To Bind Socket\n"); return -1;}
    __LOG_INFO__("Cli Socket Bind Successfull\n");

    if(listen(cli_socket , 100) < 0)
        {__LOG_WARN__("Failed To Listen Socket\n"); return -1;}
    __LOG_INFO__("Cli Listen Successfull\n");

    return 1;
}

void
run_cli_daemon()
{
    __CheckErr(cli_socket <= 0, "Cli Socket Is Not Initialized\n");
    __LOG_INFO__("Start Running Cli Daemon\n");

    int conn;
    while((conn = accept(cli_socket, NULL, 0)))
    {
        __LOG_INFO__("New Connection Accepted : %d\n", conn);
        /* connection handler thread */
    }
}