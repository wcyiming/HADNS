#include "hashlist.h"


int HashList_Judge(const HashURL url1, const HashURL url2)
{
    return strncmp(url1, url2, MAX_IPV4_LENGTH) == 0;
}

void HashList_CopyUrl(HashURL *pDst, const HashURL url)
{
    strncpy(*pDst, url, MAX_URL_LENGTH);
}

void HashList_CopyIP(HashIP *pDst, const HashIP ip) 
{
    strncpy(*pDst, ip, MAX_IPV4_LENGTH);
}

Hash_Node* HashList_CreatNode(const HashURL url, const HashIP ip,const int ttl)
{
    Hash_Node *node = (Hash_Node *)malloc(sizeof(Hash_Node));

    if (!node) {
        return node;
    }
    HashList_CopyUrl(&node->nodeURL, url);
    HashList_CopyIP(&node->nodeIP, ip);
    if(ttl!=0)
    {
		if(ttl >MAX_TTL)
         node->ttl_time = time(NULL) + MAX_TTL;
		else
		node->ttl_time = time(NULL) + ttl;
    }
    else
    {
        node->ttl_time = 0;
    }   
    node->next  = NULL;
    node->prev_next = NULL;
    return node;
}



int HashList_InsertNode(hashListHead *h, Hash_Node *node)
{
    if (!node || !h) {
        return 0;
    }
    node->next = h->first;

    if (h->first) {
        h->first->prev_next = &node->next;
    }
    h->first = node;
    return 1;
}

Hash_Node* HashList_Find(const hashListHead *h, const HashURL url)
{
    Hash_Node *iter = h->first;

    while (iter) {
        if (HashList_Judge(iter->nodeURL, url) &&
        (iter->ttl_time == 0 || iter->ttl_time > time(NULL))  )         //判断生存时间是否超时
        {
            break;
        }
        iter = iter->next;
    }
    return iter;
}


int HashList_DeleteNode(hashListHead *h, Hash_Node *node)
{
    if (!node) {
        return 1;
    }

    if (h->first == node) {
        h->first = node->next;
    }

    // 更改node前驱节点的后链节
    if (node->prev_next) {
        *node->prev_next = node->next;
    }

    if (node->next) {
        node->next->prev_next = node->prev_next;
    }
    free(node);
    return 1;
}

int HashList_Clear(hashListHead *h)
{
    Hash_Node *iter = h->first;
    Hash_Node *tmp;

    while (iter) {
        tmp  = iter;
        iter = iter->next;
        free(tmp);
    }
    return 1;
}
