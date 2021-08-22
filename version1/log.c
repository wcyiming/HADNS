#include "log.h"

static const char* getTimeStr()//时间信息生成
{
    time_t now = time(NULL);
    strftime(timeString,TIME_STR_LENGTH, logTime,localtime(&now));
    return timeString;
}


static void log_print(const char *fmt, va_list arg)     //输出
{
    fprintf(stdout, "%s ", getTimeStr());
    vfprintf(stdout, fmt, arg);
    fflush(stdout);

    fprintf(log_file, "%s ", getTimeStr());
    vfprintf(log_file, fmt, arg);
	fflush(log_file);
}

static void log_print_no(const char *fmt, va_list arg) //无时间信息输出
{
    vfprintf(stdout, fmt, arg);
    fflush(stdout);

    vfprintf(log_file, fmt, arg);
	fflush(log_file);
}

void log_debug(const char *fmt, ...)  //等级3需要输出
{
    if(log_type != 3) {
        return ;
    }
    va_list arg;
    va_start(arg, fmt);
    log_print(fmt, arg);
    va_end(arg);
}

void log_notime(const char *fmt, ...)  //日常输出
{
    va_list arg;
    va_start(arg, fmt);
    log_print_no(fmt, arg);
    va_end(arg);
}


void log_info(const char *fmt, ...) //等级2输出
{
    if(log_type == 1) {
        return ;
    }
    va_list arg;
    va_start(arg, fmt);
    log_print(fmt, arg);
    va_end(arg);
}


void setLogType(int roamType) //更改log等级
{
    log_type = roamType;
    log_debug("<debug> :: log type has been set to %d.\n", log_type);
    return;
}

int getLogType()            //向其他文件返回log等级
{
    return log_type;
}