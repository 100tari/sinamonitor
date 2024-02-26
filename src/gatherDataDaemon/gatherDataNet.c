#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "gatherDataDaemon/gatherDataNet.h"
#include "gatherDataDaemon/gatherDataBase.h"
#include "common/logger.h"

#define rx_bytes_path(intrfc)         "/sys/class/net/" TOSTRING(intrfc) "/statistics/rx_bytes"
#define tx_bytes_path(intrfc)         "/sys/class/net/" TOSTRING(intrfc) "/statistics/tx_bytes"

static int    rx_bytes_file;
static int    tx_bytes_file;
static short is_data_net_init;

int
init_data_NET()
{
    if((rx_bytes_file = open(rx_bytes_path(INTERFACE_NAME), O_RDONLY)) < 0)
        { __LOG_WARN__("Failed To Open %s\n", rx_bytes_path(INTERFACE_NAME)); return -1;}

    if((tx_bytes_file = open(tx_bytes_path(INTERFACE_NAME), O_RDONLY)) < 0)
        { __LOG_WARN__("Failed To Open %s\n", tx_bytes_path(INTERFACE_NAME)); return -1;}      

    is_data_net_init = 1;
    __LOG_DEBG__("Init Data Net Successfull, %s And %s Are Opened\n", TOSTRING(rx_bytes_file), TOSTRING(tx_bytes_file));

    return 1;
}

int
destroy_data_NET()
{
    __CheckNull(rx_bytes_file);
    __CheckNull(tx_bytes_file);

    if(!close(rx_bytes_file))
        { __LOG_WARN__("Failed To Close %s\n", TOSTRING(rx_bytes_file)); return -1;}  

    if(!close(tx_bytes_file))
        { __LOG_WARN__("Failed To Close %s\n", TOSTRING(tx_bytes_file)); return -1;}  

    is_data_net_init = 0;
    __LOG_INFO__("Destroy Data Net Successfull, %s And %s Are Closed\n", TOSTRING(rx_bytes_file), TOSTRING(tx_bytes_file));

    return 1;
}

static int 
read_data_NET(NET_Gather_Handler *ngh)
{
    if(!is_data_net_init)
        {__LOG_WARN__("Data Net Was Not Initialized\n"); return -1;}

    __CheckNull(ngh);
    char c[1];
    ngh->rx = 0;
    ngh->tx = 0;

    if(lseek(rx_bytes_file, 0, SEEK_SET) != 0)
        {__LOG_WARN__("rx_bytes_file lseek Failed\n"); return -1;}
    if(lseek(tx_bytes_file, 0, SEEK_SET))
        {__LOG_WARN__("tx_bytes_file lseek Failed\n"); return -1;}

    while(read(rx_bytes_file, c, 1) == 1) 
        {if(c[0] >= '0' && c[0] <= '9') ngh->rx = ngh->rx*10 + (c[0] - '0'); }
    
    while(read(tx_bytes_file, c, 1) == 1)   
        {if(c[0] >= '0' && c[0] <= '9') ngh->tx = ngh->tx*10 + (c[0] - '0'); }


    ngh->time_stamp = time(NULL);

    return 1;
}

static int 
update_data_NET()
{
    if(!is_data_net_init)
        {__LOG_WARN__("Data Net Was Not Initialized\n"); return -1;}

    NET_Gather_Handler frst_ngh;
    NET_Gather_Handler scnd_ngh;

    if(read_data_NET(&frst_ngh) < 0)
        {__LOG_WARN__("Failed To Read Data Net\n"); return -1;}
    sleep(UPDATE_NET_DATA_INTERVAL);
    if(read_data_NET(&scnd_ngh) < 0)
        {__LOG_WARN__("Failed To Read Data Net\n"); return -1;}

    __LOG_DEBG__("frst_ngh = (%ld, %ld) , scnd_ngh = (%ld, %ld)\n", frst_ngh.rx, frst_ngh.tx, scnd_ngh.rx, scnd_ngh.tx);    

    dataBase db;
    double diff_time = difftime(scnd_ngh.time_stamp, frst_ngh.time_stamp);
    db.data_rxTraffic = (scnd_ngh.rx - frst_ngh.rx)/diff_time;
    db.data_txTraffic = (scnd_ngh.tx - frst_ngh.tx)/diff_time;

    __LOG_DEBG__("rxTraffic = %g, txTraffic = %g\n", db.data_rxTraffic, db.data_txTraffic);

    update_data_base(&db);
    __LOG_INFO__("Data Net Updated Successfully\n");

    return 1;
}

void
run_data_NET()
{
    if(!is_data_net_init)
        {__LOG_WARN__("Can Not Run Data Net, It Is Not Initialized\n"); return;}
    __LOG_INFO__("Start Running Data Net\n");

    while(1)
    {
        if(update_data_NET() < 0)
            destroy_data_NET();
    }
}
