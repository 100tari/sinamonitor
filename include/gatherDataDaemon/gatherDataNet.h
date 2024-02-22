#ifndef _GATHER_DATA_NET_H_
#define _GATHER_DATA_NET_H_

/*
    NET data:   tx traffic
                rx traffic
*/

#include <time.h>
#include <stdint.h>

/* this macro specifies the interface which we will monitor */
#define INTERFACE_NAME                  wlan0

/* this macro specifies the interval time NET data updates */
#define UPDATE_NET_DATA_INTERVAL        5

/* this struct holds needed information to gather NET data */
typedef struct NET_Gather_Handler_s
{
    uint32_t          rx;
    uint32_t          tx;
    time_t          time_stamp;
} NET_Gather_Handler;

int                                     init_data_NET();
void                                    run_data_NET();
int                                     destroy_data_NET();

#endif // _GATHER_DATA_NET_H_