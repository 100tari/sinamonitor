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

    return 0;
}