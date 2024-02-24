# A multi daemon monitoring agent; Based on net-snmp and libcli

## Gather Net Data Notes
At the moment it only gathers rx and tx traffic data and more data will be added in next releases
Make sure INTERFACE_NAME in gatherDataNet.h is exactly same as the interface name you want to monitor (no need for double quote for the name)
Also you can change the update interval of traffic data by changing UPDATE_NET_DATA_INTERVAL in gatherDataNet.h

## Logger Notes
The logger used in this project is a macro-logger; which means it's implemented just by using macros, therefor it has less flexibility than functional-loggers. if you wnat change some configuration you have to do it before compiling in logger.h file.

## Snmp-Agent Notes
Before running the program, you have to copy the basic .conf file to .snmp directory in your /home.
The name of conf file must be exactly same as the SNMP_MASTER_AGENT_NAME macro in snmpDaemon.h 
Community name is public and agent can serve only localhost's requests.
You can find MIB tree detail in snmpMibTree.h.
example: snmpget -v 2c -c public localhost 1.3.6.1.4.1.1212.1.1

## Cli-Agent Notes 
You can edit some properties like port number, hostname and ... in cliDaemon.h
Commands could be found in cliCommandNet.h 

