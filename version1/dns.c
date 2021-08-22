#include "dns.h"
#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char *argv[])
{
	toLog(argc, argv);
	dnsInit();
	dnsRun();
	return 0;
}

void toLog(int argc,char * argv[])
{

	int wrongOrderFlag = 0;        //判断命令行是否符合格式

	log_file = fopen( "log.txt","w");
	if(!log_file) {
		log_debug("<debug> : log file failed open.\n");
		exit(1);
	}

	switch (argc)
	{
	case 1:                                             //无信息输出
	    setLogType(1);
		strncpy(DnsFileName, "dnsrelay.txt", MAX_FILE_NAME_LENGTH);
		log_debug("<debug> : DNS file name has been set to %s.\n", DnsFileName);
		strncpy(toDnsSeverIp,ConstIp,MAX_IPV4_LENGTH);
		log_debug("<debug> : server DNS IP has been set to %s.\n", toDnsSeverIp);
		break;

	case 2:
	wrongOrderFlag = 1;
	break;

	case 3:
	if (strncmp(argv[1], "-dd", 3) == 0) {                     //debug信息输出
            setLogType(3);
			strncpy(DnsFileName, "dnsrelay.txt", MAX_FILE_NAME_LENGTH);
		    log_debug("<debug> : DNS file name has been set to %s.\n", DnsFileName);
		    strncpy(toDnsSeverIp,argv[2],MAX_IPV4_LENGTH);
		    log_debug("<debug> : server DNS IP has been set to %s.\n", toDnsSeverIp);
        } 
		else {
            wrongOrderFlag = 1;
        }
        break;
	
	case 4:                                                  //仅输出info
	
    if (strncmp(argv[1], "-d", 2) == 0) {
        setLogType(2);
		strncpy(DnsFileName, argv[3], MAX_FILE_NAME_LENGTH);
		log_debug("<debug> : DNS file name has been set to %s.\n", DnsFileName);
		strncpy(toDnsSeverIp,argv[2],MAX_IPV4_LENGTH);
		log_debug("<debug> : server DNS IP has been set to %s.\n", toDnsSeverIp);
    } 
	else {
        wrongOrderFlag = 1;
    }
    break;

	default:
	wrongOrderFlag = 1;
	break;
	}

	if(wrongOrderFlag)	{
		log_notime("no such order,please read the user's pdf to get help\n");
		exit(0);
	}


	log_notime(
        "Setted : Log Type: %d, server DNS IP: %s, DNS file name: %s.\n",
        getLogType(),
        toDnsSeverIp,
        DnsFileName);
}

void  dnsInit()
{
	log_notime("Program starting up.\n");

	dnsCache = HashMap_Create();
	
	if(!dnsCache) {
		log_debug("<debug> :  Fail to create DNS Cache. Program exit.\n");
		exit (1);
	}

	FILE *dnsFile = fopen (DnsFileName , "r");
	fpout = fopen("dnsrelayout.txt", "w");


	while (fscanf(dnsFile, "%s%s", IPNum, url) == 2) {
            HashMap_AddNoLRU(dnsCache, url, IPNum);
            log_info("Read from file, URL: %s   IP: %s.\n", url, IPNum);
    }

	log_debug("<debug> : dnscache init success.\n");



	WSAStartup(MAKEWORD(2,2) , &wsaData);

	socketServer = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	socketClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	 if (socketServer == INVALID_SOCKET) {
            log_debug("<debug> :  socketserver creation failed. Program exit.\n");
            WSACleanup();
            exit(1);
        }
		
	 if (socketClient == INVALID_SOCKET) {
            log_debug("<debug> : socketclient creation failed. Program exit.\n");
            WSACleanup();
            exit(1); 
    }
    
	SOCKADDR_IN ListenAddress = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = INADDR_ANY,
		.sin_port = htons(DNS_PORT)
	};
	
	int bindFlag = bind(socketClient,
	                    (SOCKADDR *) &ListenAddress,
                        sizeof(SOCKADDR));

	if(bindFlag < 0){
		log_debug("<debug> : socketclient binding failed. Program exit.\n");
		WSACleanup();
        exit(1);
	}
	log_debug("<debug> : socket init success.\n");

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr =  inet_addr(toDnsSeverIp);
	serverAddress.sin_port = htons(DNS_PORT);

}


void dnsRun()
{
	fd_set fd;
	while (1) {
		FD_ZERO(&fd);
		FD_SET(socketClient , &fd);
		FD_SET(socketServer , &fd);
		TIMEVAL timeout = {5,0};
		int ret = select(0 , &fd ,NULL, NULL ,&timeout);

		if(ret < 0){
			log_debug("<debug> : socket select failed. Program exit.\n");
			exit(1);
		}
		if(ret > 0)
		{
			if (FD_ISSET(socketServer , &fd)) {
				recvFromServer();
			}
			if(FD_ISSET(socketClient , &fd)) {
				recvFromClient();
			}
		}
	}
}

int changeURL(char* tureURL, const char* passURL)
{
	size_t roamLength;
	while ((*passURL) != 0) {
		roamLength = (size_t)*passURL;                 //记载值
		passURL++;
		strncpy(tureURL, passURL, roamLength);
		tureURL += roamLength;
		passURL += roamLength;

		if (*passURL != 0) {
			*tureURL = '.';
			tureURL++;
		}
		else {
			*tureURL = 0;                       //结尾
		}
	}
	return 1;
}

void recvFromClient()
{
	int recvAddressLength = sizeof(SOCKADDR);
	recvLength_c = recvfrom(
		socketClient,
		buffer_c,
		MAX_DNSDATA_SIZE,
		0,
		(SOCKADDR*)&clientAddress,
		&recvAddressLength);
	if (recvLength_c <= 0) {
		log_debug("<debug> : Received wrong dnsdata, will ignored it.\n");
		return;
	}



	struct Dns_Head* const nowHead = (struct Dns_Head*) buffer_c;
	struct Answer_Write* nowAns;
	changeURL(url_c, buffer_c + DNS_HEAD_SIZE);
	Hash_Node* node = HashMap_Find(dnsCache, url_c);
	int lengthToSend;

	if (node)//dns池子里有
	{
		log_debug("<debug> : dns finded.  "
			"URL: %s   IP:%s  \n", url_c, IPNum_c);

		strncpy(IPNum_c, node->nodeIP, MAX_IPV4_LENGTH);
		int banFlag = (strncmp(IPNum_c, "0.0.0.0", MAX_IPV4_LENGTH) == 0);

		nowHead->qr = 1;

		if (banFlag)             //污染
		{
			nowHead->Total_Answer_RRs = htons(0x0000);
			nowHead->Rcode = 3;
			lengthToSend = recvLength_c;
		}
		else {

			nowAns = (struct Answer_Write*) (buffer_c + recvLength_c);
			nowAns->name = htons(0xc00c);
			nowAns->type = htons(0x0001);
			nowAns->class = htons(0x0001);
			nowAns->ttl_low = (unsigned short)REPLY_TTL;
			nowAns->ttl_high = (unsigned short)(REPLY_TTL >> 16);
			nowAns->ip_length = htons(0x0004);
			nowAns->ip = inet_addr(IPNum_c);
			nowHead->Total_Answer_RRs = htons(0x0001);
			lengthToSend = recvLength_c + sizeof(struct Answer_Write);
		}
		log_debug("%d \n", nowHead->qr);
		int sendLength;
		sendLength = sendto(
			socketClient,
			buffer_c,
			lengthToSend,
			0,
			(SOCKADDR*)&clientAddress,
			sizeof(SOCKADDR));
		log_info("Replied to client IP: %s,  URL: %s  IP: %s .\n",
			inet_ntoa(clientAddress.sin_addr), url_c, IPNum_c);
		return;

	}




	//中继
	int relay_cnt;
	for (relay_cnt = 0; relay_cnt != MAX_RELAY_NUM; relay_cnt++)
	{
		if (relayCache[relay_cnt].flag || (relayCache[relay_cnt].out_time <= time(NULL)))
		{
			break;
		}
	}
	if (relay_cnt == MAX_RELAY_NUM)
	{
		log_debug("<debug> : no relay left, skipped request.\n");
		return;
	}

	relayCache[relay_cnt].relay_id = nowHead->id;
	relayCache[relay_cnt].relay_address = clientAddress;
	relayCache[relay_cnt].flag = 0;
	relayCache[relay_cnt].out_time = time(NULL) + MAX_DNS_TIME;


	int nowID = relay_cnt;

	nowHead->id = nowID;
	lengthToSend = recvLength_c;

	int sendLength;
	sendLength = sendto(
		socketServer,
		buffer_c,
		lengthToSend,
		0,
		(SOCKADDR*)&serverAddress,
		sizeof(SOCKADDR));

	log_info("DNS Not Found IN Relay :\n "
		"URL: %s not found, asking remote DNS server  ID: %d (Original ID: %d).\n",
		url_c,
		nowID,
		relayCache[relay_cnt].relay_id);

	return;

}


void recvFromServer()
{
	int recvAddressLength = sizeof(SOCKADDR);
	recvLength_s = recvfrom(
		socketServer,
		buffer_s,
		MAX_DNSDATA_SIZE,
		0,
		(SOCKADDR*)&roamAddress,
		&recvAddressLength);
	if (recvLength_s <= 0) {
		log_debug("<debug> : Received wrong dnsdata from server, will ignored it.\n");
		return;
	}



	struct Dns_Head* const  nowHead = (struct Dns_Head*) buffer_s;
	char* nowPtr = buffer_s + DNS_HEAD_SIZE;
	struct Answer_Read* nowAns;
	int lengthToSend, sendLength;

	unsigned short nowID = nowHead->id;
	int ansRRs = ntohs(nowHead->Total_Answer_RRs);
	int queRRs = ntohs(nowHead->Total_Quesions);

	while (queRRs--) {
		changeURL(url_s, nowPtr);
		//跳过域名部分
		while (*nowPtr) {
			nowPtr += *nowPtr + 1;
		}
		nowPtr += 5;   //跳过0，type class
	}

	while (ansRRs--) {


		//地址指针
		if ((unsigned char)(*nowPtr) == 0xc0) {
			nowPtr += 2;
		}
		//跳过域名
		else {
			while (*nowPtr) {
				nowPtr += *nowPtr + 1;
			}
			nowPtr++;
		}

		nowAns = (struct Answer_Read*) nowPtr;

		if (nowAns->type != htons(0x0001))                             //跳过非A（IPV4）
		{
			nowPtr = (char*)&nowAns->data + htons(nowAns->ip_length);
			continue;
		}

		strncpy(IPNum_s, inet_ntoa(*(IN_ADDR*)(&nowAns->data)), MAX_IPV4_LENGTH);
		int ttl = ntohs(nowAns->ttl_high);
		ttl = ttl << 16;
		ttl += ntohs(nowAns->ttl_low);
		HashMap_Insert(dnsCache, url_s, IPNum_s, ttl);                  //插入cache，并写入文件
		log_debug("<debug> : URL: %s   IP: %s add to dnscache\n",
			url_s, IPNum_s);
		break;

	}


	relayCache[nowID].flag = 1;
	if (relayCache[nowID].out_time <= time(NULL))
	{
		return ;
	}
	nowHead->id = relayCache[nowID].relay_id;
	lengthToSend = recvLength_s;
	sendLength = sendto(socketClient,
		buffer_s,
		lengthToSend,
		0,
		(SOCKADDR*)&relayCache[nowID].relay_address,
		sizeof(SOCKADDR));

	log_info("Replied to client (IP: %s) with URL: %s  IP: %s\n",
		inet_ntoa(relayCache->relay_address.sin_addr),
		url_s,
		IPNum_s);

	return;
}