#ifndef GLOBAL_H
#define GLOBAL_H

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS 


# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <WinSock2.h>
# include <time.h>


#define DNS_PORT 53              //DNS端口号
#define MAX_URL_LENGTH 256       //URL字段长度最大为256
#define MAX_IPV4_LENGTH 16       //IPv4地址的字符串长
#define MAX_DNSDATA_SIZE 1024   //DNS报文的最大长度

#define MAX_RELAY_NUM 64       //最大并发查询处理的数量
#define MAX_CACHE_NUM 100      //LRU缓存的最大缓存量
#define MAX_DNS_TIME 10         //UDP数据的超时时间
#define MAX_FILE_NAME_LENGTH 255 //Windows下最大文件名长度
#define DNS_HEAD_SIZE 12       //DNS固定报头的长度
#define REPLY_TTL 0          //回复报文里填写的的TTL
#define HASH_TABLE_SIZE 1024     //存储DNS哈希表的长度
#define MAX_TTL 120               //TTL最大值
FILE* log_file;                  //LOG日志输出文件
FILE* fpout;                     //存进cache的输出文件

#endif