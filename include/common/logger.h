#ifndef _COMMON_LOGGER_H_
#define _COMMON_LOGGER_H_

#ifdef __cplusplus
extern "C" {                            /* now we don't need this but in future it may be useful */
#endif // __cplusplus

#include <stdio.h>
#include <stdlib.h>

/* configuration macros */
#define DEFAULT_LOG_LEVEL       4       /* a nonnegetive integer; values are listed in bellow in Log Levels */
#define LOG_META_DATA           1       /* set to nonzero if you want meta data (line, function, time, ...) displays */     
#define LOG_COLORIC             1       /* set to nonzero if you want to log in coloric text */                          
#define EXIT_IN_ERR             1       /* set to nonezero if you want exit when error happens */
#define LOG_MAKE_CONF_ON        1       /* used in redefining config macro within the program */
#define LOG_MAKE_CONF_OFF       0       /* used in redefining config macro within the program */

/* CHECK macros are to making sure if all needed macros are defined and not accidently undefied */
#define __LOGGER_CHECK_CONF_MACRO__    defined(DEFAULT_LOG_LEVEL)&&defined(LOG_META_DATA)&&defined(LOG_COLORIC)&&defined(EXIT_IN_ERR)&&defined(LOG_MAKE_CONF_ON)&&defined(LOG_MAKE_CONF_OFF)

/* helper macros */
#define TOSTRING(X)             #X
#define ESC_SEQ(colr)           "\e[" TOSTRING(colr) "m"

/* coleric text helper macros */
#define CRED                    ESC_SEQ(31)
#define CGRN                    ESC_SEQ(32)
#define CYEL                    ESC_SEQ(33)
#define CBLU                    ESC_SEQ(34)
#define CMGN                    ESC_SEQ(35)
#define CCYN                    ESC_SEQ(36)
#define NORM                    ESC_SEQ()

/* CHECK macros are to making sure if all needed macros are defined and not accidently undefied */
#define __LOGGER_CHECK_HELPER_MACRO__  defined(TOSTRING)&&defined(ESC_SEQ)&&defined(CRED)&&defined(CGRN)&&defined(CYEL)&&defined(CBLU)&&defined(CMGN)&&defined(CCYN)&&defined(NORM)

/* Log Levels */
#define LOG_L_CRIT              0
#define LOG_L_EROR              1      
#define LOG_L_WARN              2
#define LOG_L_INFO              3
#define LOG_L_DEBG              4

/* CHECK macros are to making sure if all needed macros are defined and not accidently undefied */
#define __LOGGER_CHECK_LOGLVL_MACRO__  defined(LOG_L_CRIT)&&defined(LOG_L_EROR)&&defined(LOG_L_WARN)&&defined(LOG_L_INFO)&&defined(LOG_L_DEBG)

/* main log macros */
#define __LOG_META_DATA__(fd)     {if(LOG_META_DATA){fprintf(fd, NORM);fprintf(fd, "[%s:%d] ", __FUNCTION__, __LINE__);}}  // TODO : adding time in __LOG_META_DATA__
#define __LOG_COLORIC__(fd,CLR)   {if(LOG_COLORIC) {fprintf(fd, CLR);}}    
#define __LOG_CRIT__(...)         {if(DEFAULT_LOG_LEVEL>=LOG_L_CRIT){__LOG_META_DATA__(stderr);__LOG_COLORIC__(stderr, CMGN);fprintf(stderr,"[CRITICAL]\t");   fprintf(stderr, __VA_ARGS__);fprintf(stderr, NORM);} if(EXIT_IN_ERR) {fprintf(stderr, "Terminating...\n");exit(EXIT_FAILURE);}}
#define __LOG_EROR__(...)         {if(DEFAULT_LOG_LEVEL>=LOG_L_EROR){__LOG_META_DATA__(stderr);__LOG_COLORIC__(stderr, CRED);fprintf(stderr,"[ERROR]\t");      fprintf(stderr, __VA_ARGS__);fprintf(stderr, NORM);} if(EXIT_IN_ERR) {fprintf(stderr, "Terminating...\n");(exit(EXIT_FAILURE));}}
#define __LOG_WARN__(...)         {if(DEFAULT_LOG_LEVEL>=LOG_L_WARN){__LOG_META_DATA__(stdout);__LOG_COLORIC__(stdout, CYEL);fprintf(stdout,"[WARNING]\t");    fprintf(stdout, __VA_ARGS__);fprintf(stdout, NORM);}}
#define __LOG_INFO__(...)         {if(DEFAULT_LOG_LEVEL>=LOG_L_INFO){__LOG_META_DATA__(stdout);__LOG_COLORIC__(stdout, CGRN);fprintf(stdout,"[INFO]\t\t");     fprintf(stdout, __VA_ARGS__);fprintf(stdout, NORM);}}
#define __LOG_DEBG__(...)         {if(DEFAULT_LOG_LEVEL>=LOG_L_DEBG){__LOG_META_DATA__(stdout);__LOG_COLORIC__(stdout, CBLU);fprintf(stdout,"[DEBUG]\t");      fprintf(stdout, __VA_ARGS__);fprintf(stdout, NORM);}}

/* handy log macros */
#define __CheckErr(cond, ...)       {if((cond)) __LOG_EROR__(__VA_ARGS__);};
#define __CheckErrExit(cond, ...)   {if((cond)) __LOG_EROR__(__VA_ARGS__) exit(EXIT_FAILURE);};
#define __CheckNull(var)           __CheckErr(var==0, "Varibale %s is Null\n", #var);

/* CHECK macros are to making sure if all needed macros are defined and not accidently undefied */
#define __LOGGER_CHECK_LOGGER_MACRO__  defined(__LOG_META_DATA__)&&defined(__LOG_COLORIC__)&&defined(__LOG_CRIT__)&&defined(__LOG_EROR__)&&defined(__LOG_WARN__)&&defined(__LOG_INFO__)&&defined(__LOG_DEBG__)&&defined(__CheckErr)&&defined(__CheckErrExit)&&defined(__CheckNull)

#ifdef __cplusplus
}
#endif // __cplusplus

#if !(__LOGGER_CHECK_CONF_MACRO__)
#error "missing macro in configuration macros in logger.h"
#endif // __LOGGER_CHECK_CONF_MACRO__

#if !(__LOGGER_CHECK_HELPER_MACRO__)
#error "missing macro in helper macros in logger.h"
#endif // __LOGGER_CHECK_HELPER_MACRO__

#if !(__LOGGER_CHECK_LOGLVL_MACRO__)
#error "missing macro in log level macros in logger.h"
#endif // __LOGGER_CHECK_LOGLVL_MACRO__

#if !(__LOGGER_CHECK_LOGGER_MACRO__)
#error "missing macro in logger macros in logger.h"
#endif // __LOGGER_CHECK_LOGGER_MACRO__

/* CHECK macros are to making sure if all needed macros are defined and not accidently undefied */
#define __LOGGER_CHECK_ALL_MACRO__ __LOGGER_CHECK_CONF_MACRO__&&__LOGGER_CHECK_HELPER_MACRO__&&__LOGGER_CHECK_LOGGER_MACRO__&&__LOGGER_CHECK_LOGLVL_MACRO__

#endif // _COMMON_LOGGER_H_