#include "hashmap.h"

int HashMap_Hash(const HashURL url, int maxSize)      //��ϣ
{
    int    n = 0;
    char  *b = (char *)&n;
    size_t i;

    for (i = 0; i < strlen(url); ++i) {
        b[i % 4] ^= url[i];
    }

    return n % maxSize;
}

void HashMap_DeleteNode(HashMap *map, Hash_Node *node)       //��ϣ��ɾ���ڵ�
{
    if (!node) {
        return ;
    }
    int hash = HashMap_Hash(node->nodeURL, map->hashSize);        
    hashListHead *head = &map->hashTable[hash];
    int ret = HashList_DeleteNode(head, node);               //�Ӹ�ֵ��ϣ����ɾ���ڵ�

}

void HashMap_Destroy(HashMap *map)               //��ϣ��ر�
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++) 
    {
        HashList_Clear(&map->hashTable[i]);            //ɾ����ϣ��
    }
    free(map);
    return;
}

MapLRU* HashMap_Create()                 //��ϣ����
{
    MapLRU *map = (MapLRU *) malloc (sizeof(MapLRU));
    if(!map) {
        return NULL;
    }
    
    HashMap *hashmaps = (HashMap *)malloc(sizeof(HashMap));

    if (!hashmaps) {
        return NULL;
    }
    hashmaps->hashSize = HASH_TABLE_SIZE;
    memset(&hashmaps->hashTable, 0, sizeof(hashmaps->hashTable));
    map->hashMap=hashmaps;

    map->lru = LRU_Create();
    if(!map->lru)
    {
        HashMap_Destroy(map->hashMap);           //����ʧ�ܣ��ͷŹ�ϣ���ڴ�
        return NULL;
    }
    return map;

}

Hash_Node* HashMap_AddNoLRU(MapLRU *map ,HashURL url, HashIP ip)    //���ýڵ����
{
    Hash_Node *node = HashList_CreatNode (url, ip,0);           //������ϣ��ڵ㣬���ýڵ��ttlΪ0

    if(!node)
    {
        return NULL;
    }
    
    int  hash = HashMap_Hash(node->nodeURL, map->hashMap->hashSize);    
    hashListHead *head = &map->hashMap->hashTable[hash];

    int insertFlag = HashList_InsertNode(head, node);          //�ڸ�ֵ��ϣ���в���õ�
    
    if(!insertFlag)
    {
        free(node);
        return NULL;
    }
    return node;
}

void sourceOut(const char* fmt, ...)        //���ļ�����
{
	va_list arg;
	va_start(arg, fmt);
	vfprintf(fpout, fmt, arg);
	fflush(fpout);
	va_end(arg);
}
Hash_Node* HashMap_Insert (MapLRU *map , HashURL url, HashIP ip, int outTime)   //��ϣ�����
{
	sourceOut("%s %s %d\n",url,ip ,outTime);        //�����ļ�


    Hash_Node *node = HashList_CreatNode (url, ip, outTime);          //������ϣ��ڵ�

    if(!node)
    {
        return NULL;
    }
    
    int  hash = HashMap_Hash(node->nodeURL, map->hashMap->hashSize);          
    hashListHead *head = &map->hashMap->hashTable[hash];

    int insertFlag = HashList_InsertNode(head, node);          //�����ϣ�ڵ�
    if(!insertFlag)
    {
        free(node);
        return NULL;
    }


    if (LRU_isFull(map->lru))                                //���LRU���ˣ��������ϵ�ֵ����һ����
    {
        LRU_Node *front = LRU_Front(map->lru);               //�ҵ���һ��
        if(front)
        {
            HashMap_DeleteNode(map->hashMap , front->data);        //ɾ��LRU��һ���ڵ��Ӧ�Ĺ�ϣ���ڵĽڵ�
        }
        LRU_POP(map->lru);                                       //ɾ��LRU��һ��
    }

    insertFlag = LRU_Insert(map->lru , node);                  //����LRU��
    if(!insertFlag)
    {
        HashMap_DeleteNode(map->hashMap , node);
        return NULL;
    }
    return node;
}

Hash_Node* HashMap_Find(MapLRU *map , HashURL url)      //��ϣ�����
{

     int  hash = HashMap_Hash( url, map->hashMap->hashSize);
    const hashListHead *head = &map->hashMap->hashTable[hash];
    Hash_Node *node = HashList_Find(head, url);                     //�ڹ�ϣֵ�������в���


    if (!node) {
        return NULL;
    }

    LRU_Find(map->lru, node);
    return node;
}

