#include <pthread.h>

#include "gatherDataDaemon/gatherDataDaemon.h"
#include "gatherDataDaemon/gatherDataIpcHandler.h"
#include "gatherDataDaemon/gatherDataNet.h"

#include "common/logger.h"

int 
init_gather_data_daemon()
{
    __CheckErr(init_gather_data_ipc_handler() < 0,
        "Failed To Init Gather Data Daemon Ipc Handler\n");

    if(init_data_NET() < 0)
    {
        __LOG_WARN__("Failed To Init Gather Data Net\n");
        return -1;
    }

    __LOG_INFO__("Gather Data Daemon Started Successfully\n");
    return 1;
}

void
run_gather_data_daemon()
{
    __LOG_INFO__("Start Running Gather Data Daemon\n");

    pthread_t run_data_NET_t;
    if(pthread_create(&run_data_NET, NULL, &run_data_NET, NULL) != 0)
        {__LOG_WARN__("Creating run_data_NET Thread Failed\n");}
    else
    {
        if(pthread_detach(run_data_NET_t) != 0)
            {__LOG_WARN__("Running run_data_NET Thread Failed\n");}
        else
            {__LOG_INFO__("run_data_NET Thread Created And Runned Successfully\n");}
    }

    while(1)
    {
        __CheckErr(handle_gather_data_ipc() < 0,
            "Gather Data Ipc Handler Failed\n");
    }
}