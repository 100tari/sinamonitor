#include "common/logger.h"


static void 
test_logger()
{
#undef  EXIT_IN_ERR
#define EXIT_IN_ERR LOG_MAKE_CONF_OFF

    int *tmp = 0;    
    __LOG_CRIT__("critical condition happend -> %s = %d\n", TOSTRING(tmp), tmp);
    __LOG_EROR__("error condition happened\n");
    __LOG_WARN__("A Warning!\n");
    __LOG_INFO__("INFO is not useful :)\n");
    __LOG_DEBG__("but DEBUG is very useful\n");

#undef DEFAULT_LOG_LEVEL    
#define DEFAULT_LOG_LEVEL   LOG_L_INFO

    __LOG_DEBG__("now DEBUG will not be printed\n");
    __LOG_EROR__("but error will be printed\n");
    
#undef LOG_COLORIC
#define LOG_COLORIC LOG_MAKE_CONF_OFF

    __LOG_INFO__("noncoloric info\n");

    __CheckErr(1223 * 231 < 232 * 1222, "1223 * 231 < 232 * 1222 is not true\n");
    __CheckNull(tmp);

#undef  EXIT_IN_ERR
#define EXIT_IN_ERR LOG_MAKE_CONF_ON

    __CheckErrExit(1, "terminate program\n");

}

/*
int 
main()
{
    test_logger();
}
*/