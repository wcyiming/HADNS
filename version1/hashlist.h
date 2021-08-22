#ifndef HASHLIST_H
#define HASHLIST_H

#include "global.h"


typedef char HashURL[MAX_URL_LENGTH];
typedef char HashIP[MAX_IPV4_LENGTH];

typedef struct hashlist_node {
    HashURL    nodeURL;                   //url�洢
    HashIP     nodeIP;                    //ip�洢
    time_t ttl_time;                      //����ʱ��洢
    struct hashlist_node *next;           //������һ��
    struct hashlist_node **prev_next;       //ָ��ǰ���ڵ����һ����
} Hash_Node;

typedef struct hashlist_head {
    struct hashlist_node *first;              //����ͷ�ڵ�
} hashListHead;

Hash_Node* HashList_CreatNode(const HashURL url,const HashIP ip,const int ttl);      //��ϣ�������ڵ�
int        HashList_InsertNode(hashListHead *h,Hash_Node *node);                    //��ϣ������ڵ�
Hash_Node* HashList_Find(const hashListHead  *h,const HashURL url);                //��ϣ���ҵ��ڵ�

int        HashList_DeleteNode(hashListHead *h,Hash_Node *node);                  //��ϣ��ɾ���ڵ�
int        HashList_Clear(hashListHead *h);                                       //ɾ��

int HashList_Judge(const HashURL key1, const HashURL key2);                       //�ж������Ƿ���ͬ
void HashList_CopyUrl(HashURL *pDst, const HashURL url);                          //�ַ�������url
void HashList_CopyIP(HashIP *pDst, const HashIP ip) ;                             //�ַ�������ip

#endif