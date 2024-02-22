#include <libcli.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#include "cliDaemon/cliDaemon.h"
#include "cliDaemon/cliCommandNet.h"

#include "common/logger.h"

static int cli_socket;

static void 
init_cli(struct cli_def **cli)
{
    struct cli_command *get_cmnd, *tx_cmnd, *rx_cmnd;
    *cli = cli_init();

    cli_set_hostname(*cli, CLI_HOSTNAME);
    cli_set_banner(*cli, CLI_BANNER);

    get_cmnd = cli_register_command(*cli, NULL, "get", NULL, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    tx_cmnd = cli_register_command(*cli, get_cmnd, "tx", NULL, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    rx_cmnd = cli_register_command(*cli, get_cmnd, "rx", NULL, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, NULL);
    cli_register_command(*cli, tx_cmnd, "traffic", &_get_tx_traffic_command , PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Get Tx Traffic");
    cli_register_command(*cli, rx_cmnd, "traffic", &_get_rx_traffic_command, PRIVILEGE_UNPRIVILEGED, MODE_EXEC, "Get Rx Traffic");
}

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

static int 
cli_connection_handler(void *arg)
{   
    int *conn = (int*) arg;

    struct cli_def *cli;
    init_cli(&cli);
    __LOG_INFO__("Cli Init Succesull For Connection :%d\n",*conn);
    cli_loop(cli, *conn);

    cli_done(cli);
    pthread_exit(NULL);
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
        
        pthread_t pid;
        if(pthread_create(&pid, NULL, &cli_connection_handler,(void*) &conn) != 0)
            pthread_detach(pid);
        __LOG_INFO__("Thread Created For Connection : %d\n", conn);
    }
}
