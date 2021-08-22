#include "log.h"

static const char* getTimeStr()//ʱ����Ϣ����
{
    time_t now = time(NULL);
    strftime(timeString,TIME_STR_LENGTH, logTime,localtime(&now));
    return timeString;
}


static void log_print(const char *fmt, va_list arg)     //���
{
    fprintf(stdout, "%s ", getTimeStr());
    vfprintf(stdout, fmt, arg);
    fflush(stdout);

    fprintf(log_file, "%s ", getTimeStr());
    vfprintf(log_file, fmt, arg);
	fflush(log_file);
}

static void log_print_no(const char *fmt, va_list arg) //��ʱ����Ϣ���
{
    vfprintf(stdout, fmt, arg);
    fflush(stdout);

    vfprintf(log_file, fmt, arg);
	fflush(log_file);
}

void log_debug(const char *fmt, ...)  //�ȼ�3��Ҫ���
{
    if(log_type != 3) {
        return ;
    }
    va_list arg;
    va_start(arg, fmt);
    log_print(fmt, arg);
    va_end(arg);
}

void log_notime(const char *fmt, ...)  //�ճ����
{
    va_list arg;
    va_start(arg, fmt);
    log_print_no(fmt, arg);
    va_end(arg);
}


void log_info(const char *fmt, ...) //�ȼ�2���
{
    if(log_type == 1) {
        return ;
    }
    va_list arg;
    va_start(arg, fmt);
    log_print(fmt, arg);
    va_end(arg);
}


void setLogType(int roamType) //����log�ȼ�
{
    log_type = roamType;
    log_debug("<debug> :: log type has been set to %d.\n", log_type);
    return;
}

int getLogType()            //�������ļ�����log�ȼ�
{
    return log_type;
}