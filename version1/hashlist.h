#ifndef HASHLIST_H
#define HASHLIST_H

#include "global.h"


typedef char HashURL[MAX_URL_LENGTH];
typedef char HashIP[MAX_IPV4_LENGTH];

typedef struct hashlist_node {
    HashURL    nodeURL;                   //url存储
    HashIP     nodeIP;                    //ip存储
    time_t ttl_time;                      //生存时间存储
    struct hashlist_node *next;           //链表下一个
    struct hashlist_node **prev_next;       //指向（前驱节点的下一个）
} Hash_Node;

typedef struct hashlist_head {
    struct hashlist_node *first;              //链表头节点
} hashListHead;

Hash_Node* HashList_CreatNode(const HashURL url,const HashIP ip,const int ttl);      //哈希链创建节点
int        HashList_InsertNode(hashListHead *h,Hash_Node *node);                    //哈希链插入节点
Hash_Node* HashList_Find(const hashListHead  *h,const HashURL url);                //哈希链找到节点

int        HashList_DeleteNode(hashListHead *h,Hash_Node *node);                  //哈希链删除节点
int        HashList_Clear(hashListHead *h);                                       //删链

int HashList_Judge(const HashURL key1, const HashURL key2);                       //判断域名是否相同
void HashList_CopyUrl(HashURL *pDst, const HashURL url);                          //字符串插入url
void HashList_CopyIP(HashIP *pDst, const HashIP ip) ;                             //字符串插入ip

#endif