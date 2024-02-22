#include <pthread.h>

#include "gatherDataDaemon/gatherDataDaemon.h"
#include "gatherDataDaemon/gatherDataIpcHandler.h"
#include "gatherDataDaemon/gatherDataNet.h"

#include "common/logger.h"

/*
    initialize ipc handler and modules
    1 on success, -1 if one of modules fails 
*/
int 
init_gather_data_daemon()
{
    /* if ipc handler fails, the program will be unuseful and must be terminated */
    if(init_gather_data_ipc_handler() < 0)
        {__LOG_CRIT__("Failed To Init Gather Data Daemon Ipc Handler\n"); exit(1);}

    /* if NET module fails, program can keep running */
    if(init_data_NET() < 0)
        { __LOG_WARN__("Failed To Init Gather Data Net\n"); return -1;}

    __LOG_INFO__("Gather Data Daemon Started Successfully\n");
    return 1;
}

/*
    run daemon by running ipc handler and all modules
*/
void
run_gather_data_daemon()
{
    __LOG_INFO__("Start Running Gather Data Daemon\n");

    pthread_t run_data_NET_t;
    if(pthread_create(&run_data_NET_t, NULL, &run_data_NET, NULL) != 0)     /* if running NET modules failes, program can keep running */
        {__LOG_WARN__("Creating run_data_NET Thread Failed\n");}
    else
    {
        if(pthread_detach(run_data_NET_t) != 0)
            {__LOG_WARN__("Running run_data_NET Thread Failed\n");}
        else
            {__LOG_INFO__("run_data_NET Thread Created And Runned Successfully\n");}
    }

    while(1)
        if(handle_gather_data_ipc() < 0)                                    /* if running ipc fails, program must be terminated */
            {__LOG_CRIT__("Gather Data Ipc Handler Failed\n"); exit(1);}
}