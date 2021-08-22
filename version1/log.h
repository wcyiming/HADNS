#ifndef LOG_H
#define LOG_H


#include "global.h"
#include <stdarg.h>

#define TIME_STR_LENGTH  50


static int log_type;                                       //log等级
static char timeString[TIME_STR_LENGTH];                   //时间信息字符串
static const char logTime[] = "%Y/%m/%d %H:%M:%S :";       //时间信息格式


static const char* getTimeStr();                             //时间信息生成
static void log_print(const char *fmt,va_list arg);          //输出
static void log_print_no(const char *fmt, va_list arg);      //无时间信息输出
void log_debug(const char *fmt,...);                         //等级3需要输出
void log_notime(const char *fmt,...);                        //日常输出
void log_info(const char *fmt, ...);                         //等级2输出
void setLogType(int roamType);                               //更改log等级
int getLogType();                                            //向其他文件返回log等级






#endif