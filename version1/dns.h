#ifndef DNS_H
#define DNS_H


#include "global.h"
#include "log.h"
#include "hashmap.h" 

//dns报文头文件结构
struct Dns_Head{
 	unsigned id : 16;       
	unsigned rd : 1;       
	unsigned tc : 1;        
	unsigned aa : 1;        
	unsigned opcode : 4;        //操作码
	unsigned qr : 1;           //请求/回复
	unsigned Rcode : 4;        //响应码
	unsigned cd : 1;        
	unsigned ad : 1;        
	unsigned z : 1;        
	unsigned ra : 1;        
    unsigned Total_Quesions : 16;         //问题数
    unsigned Total_Answer_RRs : 16;       //回答资源记录数
    unsigned Total_Authority_RRs : 16;    //权威资源记录数
    unsigned Total_Additional_RRs : 16;    //附加资源记录数
};

//自制报文回复答案
struct  Answer_Write
{
    unsigned short name;
    unsigned short type;
    unsigned short class;
    unsigned short ttl_low;      //ttl报文32位有对齐问题
    unsigned short ttl_high;
    unsigned short ip_length;
    unsigned long ip;
};

//从服务端读取回复

struct Answer_Read
{
    unsigned short type;
    unsigned short class;
    unsigned short ttl_low;
    unsigned short ttl_high;
    unsigned short ip_length;
    unsigned short data;     //读取前两个字节的数据
};

//并发池
struct relay_cache{
    unsigned short relay_id;
    SOCKADDR_IN relay_address;
    int     flag;
    time_t  out_time;
};

static char DnsFileName [MAX_FILE_NAME_LENGTH];
static char toDnsSeverIp [MAX_IPV4_LENGTH];

static char IPNum [MAX_IPV4_LENGTH];
static char url [MAX_URL_LENGTH];
static char buffer_c [MAX_DNSDATA_SIZE];
static char oldUrl [MAX_URL_LENGTH];
static char url_c [MAX_URL_LENGTH];
static char IPNum_c [MAX_IPV4_LENGTH];
static char buffer_s [MAX_DNSDATA_SIZE];
static char url_s [MAX_URL_LENGTH];
static char IPNum_s [MAX_IPV4_LENGTH];
static char ConstIp[] = "202.106.0.20";
static MapLRU *dnsCache;

static WSADATA wsaData;
static SOCKET socketServer;  //上级通讯
static SOCKET socketClient;  //下级通讯
static SOCKADDR_IN serverAddress;
static SOCKADDR_IN clientAddress;
static SOCKADDR_IN roamAddress;
static int recvLength_s;
static int recvLength_c;         //报文长度
 struct relay_cache  relayCache [MAX_RELAY_NUM];

void recvFromClient ();  //从下级接受
void recvFromServer ();  //从上级接受
int changeURL(char *tureURL, const char *passURL);//将域名从buf转为常见格式


void toLog(int argc, char *argv[]);//建造管理日志
void dnsInit();  //dns初始化
void dnsRun();  //dns运行


#endif