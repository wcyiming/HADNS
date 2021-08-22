#ifndef HASHMAP_H
#define HASHMAP_H

#include "global.h"
#include "hashlist.h"
#include "LRU.h"
#include <stdarg.h>

typedef struct hash_map
{
    int hashSize;                                    //哈希表最大值
    hashListHead  hashTable[HASH_TABLE_SIZE];        //哈希表链
} HashMap;

typedef struct hash_map_LRU
{
    HashMap *hashMap;                               //哈希表
    LRU_Cache   *lru;                               //LRU
} MapLRU;

MapLRU* HashMap_Create();                                   //哈希表建立
Hash_Node* HashMap_Find(MapLRU *map , HashURL url);         //哈希表查找
Hash_Node* HashMap_Insert (MapLRU *map , HashURL url, HashIP ip, int outTime);         //哈希表插入
Hash_Node* HashMap_AddNoLRU(MapLRU *map ,HashURL url, HashIP ip);                      //永久节点插入

int HashMap_Hash(const HashURL url, int maxSize);                                     //哈希
void HashMap_DeleteNode(HashMap *map, Hash_Node *node);                               //哈希表删除节点
void HashMap_Destroy(HashMap *map);                                                   //哈希表关闭
void sourceOut(const char* fmt, ...);                                                 //向文件缓存


#endif