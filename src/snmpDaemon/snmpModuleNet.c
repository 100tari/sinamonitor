#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

#include "snmpDaemon/snmpModuleNet.h"
#include "snmpDaemon/snmpMibTree.h"
#include "snmpDaemon/snmpIpcHandler.h"
#include "common/commonIpcHandler.h"

static Netsnmp_Node_Handler    handle_rxTraffic;
static Netsnmp_Node_Handler    handle_txTraffic;

void 
init_module_NET()
{
    static oid  NET_RXTRAFFIC_oid[] = 
        {MIB_DEFAULT_PART, MIB_SINACOM_OID, MIB_SINACOM_NET_OID, MIB_SINACOM_NET_RXTRAFFIC_OID};

    netsnmp_handler_registration *rxTraffic_reginfo = 
        netsnmp_create_handler_registration("NET_RXTRAFFIC", handle_rxTraffic, 
        NET_RXTRAFFIC_oid, OID_LENGTH(NET_RXTRAFFIC_oid), HANDLER_CAN_RONLY);

    netsnmp_register_instance(rxTraffic_reginfo);

    static oid  NET_TXTRAFFIC_oid[] = 
        {MIB_DEFAULT_PART, MIB_SINACOM_OID, MIB_SINACOM_NET_OID, MIB_SINACOM_NET_TXTRAFFIC_OID};

    netsnmp_handler_registration *txTraffic_reginfo = 
        netsnmp_create_handler_registration("NET_TXTRAFFIC", handle_txTraffic, 
        NET_TXTRAFFIC_oid, OID_LENGTH(NET_TXTRAFFIC_oid), HANDLER_CAN_RONLY);

    netsnmp_register_instance(txTraffic_reginfo);
}

static int 
handle_rxTraffic(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{
    char buf[MAX_RES_LEN];
    if(handle_snmp_agent_ipc(RX_TRAFFIC_REQ, buf, MAX_RES_LEN) < 0)
        {snmp_log(LOG_CRIT, "[snmpModuleNet] - Getting Data Failed\n"); return SNMP_ERR_RESOURCEUNAVAILABLE;}
    
    int rxTraffic = atoi(buf);

    if(rxTraffic == -1)
        return SNMP_ERR_RESOURCEUNAVAILABLE;

    switch(reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, 
                                     ASN_INTEGER, 
                                     &rxTraffic, 
                                     sizeof(rxTraffic));
            break;
        
        default:
            snmp_log(LOG_ERR, "Unknown Mode (%d) in %s\n", 
            reqinfo->mode, __FUNCTION__);
            return  SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}

static int 
handle_txTraffic(netsnmp_mib_handler *handler,
                   netsnmp_handler_registration *reginfo,
                   netsnmp_agent_request_info *reqinfo,
                   netsnmp_request_info *requests)
{
    char buf[MAX_RES_LEN];
    if(handle_snmp_agent_ipc(TX_TRAFFIC_REQ, buf, MAX_RES_LEN) < 0)
        {snmp_log(LOG_CRIT, "[snmpModuleNet] - Getting Data Failed\n"); return SNMP_ERR_RESOURCEUNAVAILABLE;}
    
    int txTraffic = atoi(buf);

    if(txTraffic == -1)
        return SNMP_ERR_RESOURCEUNAVAILABLE;

    switch(reqinfo->mode)
    {
        case MODE_GET:
            snmp_set_var_typed_value(requests->requestvb, 
                                     ASN_INTEGER, 
                                     &txTraffic, 
                                     sizeof(txTraffic));
            break;
        
        default:
            snmp_log(LOG_ERR, "Unknown Mode (%d) in %s\n", 
            reqinfo->mode, __FUNCTION__);
            return  SNMP_ERR_GENERR;
    }

    return SNMP_ERR_NOERROR;
}
