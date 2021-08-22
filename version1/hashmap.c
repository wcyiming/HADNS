#include "hashmap.h"

int HashMap_Hash(const HashURL url, int maxSize)      //哈希
{
    int    n = 0;
    char  *b = (char *)&n;
    size_t i;

    for (i = 0; i < strlen(url); ++i) {
        b[i % 4] ^= url[i];
    }

    return n % maxSize;
}

void HashMap_DeleteNode(HashMap *map, Hash_Node *node)       //哈希表删除节点
{
    if (!node) {
        return ;
    }
    int hash = HashMap_Hash(node->nodeURL, map->hashSize);        
    hashListHead *head = &map->hashTable[hash];
    int ret = HashList_DeleteNode(head, node);               //从该值哈希链中删除节点

}

void HashMap_Destroy(HashMap *map)               //哈希表关闭
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++) 
    {
        HashList_Clear(&map->hashTable[i]);            //删除哈希链
    }
    free(map);
    return;
}

MapLRU* HashMap_Create()                 //哈希表建立
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
        HashMap_Destroy(map->hashMap);           //建立失败，释放哈希表内存
        return NULL;
    }
    return map;

}

Hash_Node* HashMap_AddNoLRU(MapLRU *map ,HashURL url, HashIP ip)    //永久节点插入
{
    Hash_Node *node = HashList_CreatNode (url, ip,0);           //建立哈希表节点，永久节点的ttl为0

    if(!node)
    {
        return NULL;
    }
    
    int  hash = HashMap_Hash(node->nodeURL, map->hashMap->hashSize);    
    hashListHead *head = &map->hashMap->hashTable[hash];

    int insertFlag = HashList_InsertNode(head, node);          //在该值哈希链中插入该点
    
    if(!insertFlag)
    {
        free(node);
        return NULL;
    }
    return node;
}

void sourceOut(const char* fmt, ...)        //向文件缓存
{
	va_list arg;
	va_start(arg, fmt);
	vfprintf(fpout, fmt, arg);
	fflush(fpout);
	va_end(arg);
}
Hash_Node* HashMap_Insert (MapLRU *map , HashURL url, HashIP ip, int outTime)   //哈希表插入
{
	sourceOut("%s %s %d\n",url,ip ,outTime);        //存入文件


    Hash_Node *node = HashList_CreatNode (url, ip, outTime);          //建立哈希表节点

    if(!node)
    {
        return NULL;
    }
    
    int  hash = HashMap_Hash(node->nodeURL, map->hashMap->hashSize);          
    hashListHead *head = &map->hashMap->hashTable[hash];

    int insertFlag = HashList_InsertNode(head, node);          //插入哈希节点
    if(!insertFlag)
    {
        free(node);
        return NULL;
    }


    if (LRU_isFull(map->lru))                                //如果LRU满了，弹出最老的值（第一个）
    {
        LRU_Node *front = LRU_Front(map->lru);               //找到第一个
        if(front)
        {
            HashMap_DeleteNode(map->hashMap , front->data);        //删除LRU第一个节点对应的哈希表内的节点
        }
        LRU_POP(map->lru);                                       //删除LRU第一个
    }

    insertFlag = LRU_Insert(map->lru , node);                  //插入LRU中
    if(!insertFlag)
    {
        HashMap_DeleteNode(map->hashMap , node);
        return NULL;
    }
    return node;
}

Hash_Node* HashMap_Find(MapLRU *map , HashURL url)      //哈希表查找
{

     int  hash = HashMap_Hash( url, map->hashMap->hashSize);
    const hashListHead *head = &map->hashMap->hashTable[hash];
    Hash_Node *node = HashList_Find(head, url);                     //在哈希值的链表中查找


    if (!node) {
        return NULL;
    }

    LRU_Find(map->lru, node);
    return node;
}

