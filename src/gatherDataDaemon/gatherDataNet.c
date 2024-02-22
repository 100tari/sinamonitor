#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "gatherDataDaemon/gatherDataNet.h"
#include "gatherDataDaemon/gatherDataBase.h"
#include "common/logger.h"

#define rx_bytes_path(intrfc)         "/sys/class/net/" TOSTRING(intrfc) "/statistics/rx_bytes"
#define tx_bytes_path(intrfc)         "/sys/class/net/" TOSTRING(intrfc) "/statistics/tx_bytes"

static FILE    *rx_bytes_file;
static FILE    *tx_bytes_file;
static short is_data_net_init;

int
init_data_NET()
{
    if((rx_bytes_file = fopen(rx_bytes_path(INTERFACE_NAME), "r")) == NULL)
    {
        __LOG_WARN__("Failed To Open %s\n", rx_bytes_path(INTERFACE_NAME));
        return -1;
    }
    if((tx_bytes_file = fopen(tx_bytes_path(INTERFACE_NAME), "r")) == NULL)
    {
       __LOG_WARN__("Failed To Open %s\n", tx_bytes_path(INTERFACE_NAME));
        return -1;
    }
    is_data_net_init = 1;

    __LOG_INFO__("Init Data Net Successfull, %s And %s Are Opened\n", TOSTRING(rx_bytes_file), TOSTRING(tx_bytes_file));

    return 1;
}

static int
destroy_data_NET()
{
    __CheckNull(rx_bytes_file);
    __CheckNull(tx_bytes_file);

    if(!fclose(rx_bytes_file))
    {
        __LOG_WARN__("Failed To Close %s\n", TOSTRING(rx_bytes_file));
        rx_bytes_file = 0;
        return -1;
    }   rx_bytes_file = 0;

    if(!fclose(tx_bytes_file))
    {
        __LOG_WARN__("Failed To Close %s\n", TOSTRING(tx_bytes_file));
        tx_bytes_file = 0;
        return -1;
    }   tx_bytes_file = 0;

    is_data_net_init = 0;
    __LOG_INFO__("Destroy Data Net Successfull, %s And %s Are Closed\n", TOSTRING(rx_bytes_file), TOSTRING(tx_bytes_file));

    return 1;
}

static int 
read_data_NET(NET_Gather_Handler *ngh)
{
    if(!is_data_net_init)
        return -1;

    __CheckNull(ngh);

    if((fscanf(rx_bytes_file, "%lf", &ngh->rx)) != 1 || fseek(rx_bytes_file, 0, SEEK_SET) != 0)
    {
        __LOG_WARN__("Failed To Read %s\n", TOSTRING(rx_bytes_file));
        return -1;
    }

    if((fscanf(tx_bytes_file, "%lf", &ngh->tx)) != 1 || fseek(tx_bytes_file, 0, SEEK_SET) != 0)
    {
        __LOG_WARN__("Failed To Read %s\n", TOSTRING(tx_bytes_file));
        return -1;
    }
    fseek(tx_bytes_file, 0, SEEK_SET);
    
    ngh->time_stamp = time(NULL);

    return 1;
}

static int 
update_data_NET()
{
    if(!is_data_net_init)
        return -1;

    NET_Gather_Handler frst_ngh;
    NET_Gather_Handler scnd_ngh;

    if(read_data_NET(&frst_ngh) < 0)
        return -1;
    sleep(UPDATE_NET_DATA_INTERVAL);
    if(read_data_NET(&scnd_ngh) < 0)
        return -1;

    dataBase db;
    double diff_time = difftime(scnd_ngh.time_stamp, frst_ngh.time_stamp);
    db.data_rxTraffic = (frst_ngh.rx - scnd_ngh.rx)/diff_time;
    db.data_txTraffic = (frst_ngh.tx - scnd_ngh.tx)/diff_time;

    update_data_base(&db);
    __LOG_INFO__("Data Net Updated Successfully\n");

    return 1;
}

void
run_data_NET()
{
    if(!is_data_net_init)
    {
        __LOG_WARN__("Can Not Run Data Net, It Is Not Initialized\n");
        return;
    }

    __LOG_INFO__("Start Running Data Net\n");

    while(1)
    {
        if(update_data_NET() < 0)
        {
            destroy_data_NET();
        }
    }
}
