#ifndef LOG_H
#define LOG_H


#include "global.h"
#include <stdarg.h>

#define TIME_STR_LENGTH  50


static int log_type;                                       //log�ȼ�
static char timeString[TIME_STR_LENGTH];                   //ʱ����Ϣ�ַ���
static const char logTime[] = "%Y/%m/%d %H:%M:%S :";       //ʱ����Ϣ��ʽ


static const char* getTimeStr();                             //ʱ����Ϣ����
static void log_print(const char *fmt,va_list arg);          //���
static void log_print_no(const char *fmt, va_list arg);      //��ʱ����Ϣ���
void log_debug(const char *fmt,...);                         //�ȼ�3��Ҫ���
void log_notime(const char *fmt,...);                        //�ճ����
void log_info(const char *fmt, ...);                         //�ȼ�2���
void setLogType(int roamType);                               //����log�ȼ�
int getLogType();                                            //�������ļ�����log�ȼ�






#endif