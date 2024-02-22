#ifndef _CLI_COMMAND_NET_H_
#define _CLI_COMMAND_NET_H_

int             _get_tx_traffic_command(struct cli_def *cli,const char *command, char *argv[], int argc);
int             _get_rx_traffic_command(struct cli_def *cli,const char *command, char *argv[], int argc);

#endif // _CLI_COMMAND_NET_H_