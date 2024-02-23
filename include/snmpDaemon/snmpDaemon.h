#ifndef _SNMP_DAEMON_H_
#define _SNMP_DAEMON_H_

/* note: the name of conf file must be exactly same as this macro */
#define SNMP_MASTER_AGENT_NAME              "master-agent"

int                     init_snmp_daemon(const char* agentName);
void                    run_snmp_daemon(const char* agentName);

#endif // _SNMP_DAEMON_H_