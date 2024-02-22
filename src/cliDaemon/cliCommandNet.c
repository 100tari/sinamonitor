#include <libcli.h>

#include "cliDaemon/cliCommandNet.h"
#include "cliDaemon/cliIpcHandler.h"
#include "common/commonIpcHandler.h"
#include "common/logger.h"

int
_get_tx_traffic_command(struct cli_def *cli,const char *command, char *argv[], int argc)
{
    char res[MAX_RES_LEN] = {0};
    if(handle_cli_ipc(TX_TRAFFIC_REQ, res, MAX_RES_LEN) < 0)
        {__LOG_WARN__("Cli Ipc Handler Failed\n"); return CLI_ERROR;}
    cli_print(cli, "tx traffic: %s bytes/seconds", res);
    return CLI_OK;
}

int
_get_rx_traffic_command(struct cli_def *cli,const char *command, char *argv[], int argc)
{
    char res[MAX_RES_LEN];
    if(handle_cli_ipc(RX_TRAFFIC_REQ, res, MAX_RES_LEN) < 0) 
        {__LOG_WARN__("Cli Ipc Handler Failed\n"); return CLI_ERROR;}
    cli_print(cli, "rx traffic: %s bytes/seconds", res);
    return CLI_OK;
}