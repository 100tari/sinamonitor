/*
                                                            _   
     ___ _ __  _ __ ___  _ __         __ _  __ _  ___ _ __ | |_ 
    / __| '_ \| '_ ` _ \| '_ \ _____ / _` |/ _` |/ _ \ '_ \| __|
    \__ \ | | | | | | | | |_) |_____| (_| | (_| |  __/ | | | |_ 
    |___/_| |_|_| |_| |_| .__/       \__,_|\__, |\___|_| |_|\__|
                        |_|                |___/                
    version: 2.0
    author: mohammad mahdi sattari
*/

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