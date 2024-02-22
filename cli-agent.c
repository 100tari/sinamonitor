/*
      ____ _     ___        _    ____ _____ _   _ _____ 
     / ___| |   |_ _|      / \  / ___| ____| \ | |_   _|
    | |   | |    | |_____ / _ \| |  _|  _| |  \| | | |  
    | |___| |___ | |_____/ ___ \ |_| | |___| |\  | | |  
     \____|_____|___|   /_/   \_\____|_____|_| \_| |_|  

    version 1.0
    author: mohammad mahdi sttari
*/

#include <unistd.h>

#include "cliDaemon/cliDaemon.h"
#include "gatherDataDaemon/gatherDataDaemon.h"
#include "common/logger.h"

int main()
{
    switch(fork())
    {
        case -1:
            __LOG_CRIT__("Failed To Fork\n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            init_gather_data_daemon();
            run_gather_data_daemon();
            break;
        default:
            init_cli_daemon();
            run_cli_daemon();
            break;
    }

    exit(EXIT_SUCCESS);
}