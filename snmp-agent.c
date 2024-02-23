#include <unistd.h>

#include "snmpDaemon/snmpDaemon.h"
#include "gatherDataDaemon/gatherDataDaemon.h"

#include "common/logger.h"

int
main()
{
    switch(fork())
    {
        case -1:
            __LOG_CRIT__("[snmp-agent] - Failed To Fork\n");
            exit(EXIT_FAILURE);
            break;
        case 0:
            init_gather_data_daemon();
            run_gather_data_daemon();
            break;
        default:
            init_snmp_daemon(SNMP_MASTER_AGENT_NAME);
            run_snmp_daemon(SNMP_MASTER_AGENT_NAME);
            break;
    }

    exit(EXIT_SUCCESS);
}