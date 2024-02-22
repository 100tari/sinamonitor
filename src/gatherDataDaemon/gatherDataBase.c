#include "gatherDataDaemon/gatherDataBase.h"

static volatile struct dataBase_s   DB;

void                        
update_data_base(dataBase *db)
{
    DB.data_rxTraffic = db->data_rxTraffic;
    DB.data_txTraffic = db->data_txTraffic;
}

void                        
get_data_base(dataBase *db)
{
    db->data_rxTraffic = DB.data_rxTraffic;
    db->data_txTraffic = DB.data_txTraffic;
}