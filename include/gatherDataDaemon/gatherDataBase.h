#ifndef _GATHER_DATA_BASE_H_
#define _GATHER_DATA_BASE_H_

#include <sys/types.h>

struct dataBase_s
{
    double              data_rxTraffic;
    double              data_txTraffic;
};

typedef struct dataBase_s   dataBase;

void                        update_data_base(dataBase *db);
void                        get_data_base(dataBase *db);

#endif // _GATHER_DATA_BASE_H_