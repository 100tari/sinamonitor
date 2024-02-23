#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <unistd.h>
#include <signal.h>

#include "snmpDaemon/snmpDaemon.h"
#include "snmpDaemon/snmpIpcHandler.h"
#include "snmpDaemon/snmpModuleNet.h"

static int is_init_snmp_daemon;

int
init_snmp_daemon(const char* agentName)
{
    if(is_init_snmp_daemon)
        {snmp_log(LOG_WARNING, "[snmpDaemon] - Snmp Daemon Has Benn Initialized Already\n"); return -1;}

    snmp_log(LOG_INFO, "[snmpDaemon] - Start Init Snmp Daemon\n");

    SOCK_STARTUP
    init_agent(agentName);
    init_module_NET();
    init_snmp(agentName);
    init_master_agent();

    if(init_snmp_ipc_handler() < 0)
        { snmp_log(LOG_CRIT, "[snmpDaemon] - IPC Socket Init Failed -> Init Snmp Agent Failed\n");
        snmp_shutdown(agentName); return -1;}

    is_init_snmp_daemon = 1;
    snmp_log(LOG_DEBUG, "[snmpDaemon] - Init Snmp Agent Successfull\n");
    return 1;
}
void
run_snmp_daemon(const char* agentName)
{
    if(!is_init_snmp_daemon)
        {snmp_log(LOG_WARNING, "[snmpDaemon] - Snmp Daemon Is Not Initialized Yet\n"); return;}

    while(1)
        agent_check_and_process(1);
    snmp_shutdown(agentName);
}

