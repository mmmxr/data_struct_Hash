#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#define TEST_HEADER printf("\n========================%s======================\n",__FUNCTION__)
size_t HashDefault(Keytype key)
{
    return key % MaxSize;
}
void HashInit(HashTable *ht)
{
    if(ht==NULL)
    {
        return;
    }
    ht->size=0;
    ht->func=HashDefault;
    size_t i=0;
    for(i=0;i<MaxSize;i++)
    {
        ht->data[i].stat=Empty;
    }
}

void HashDestroy(HashTable *ht)
{
    if(ht==NULL)
    {
        return;
    }
    ht->func=NULL;
    ht->size=0;
    size_t i=0;
    for(i=0;i<MaxSize;i++)
    {
        ht->data[i].stat=Empty;
    }
}

void HashInsert(HashTable *ht,Keytype key,Valtype value)
{
    if(ht==NULL)
    {
        return;
    }
    //规定负载因子：0.8
    if(ht->size>=0.8*MaxSize)
    {
        return;
    }
    size_t offset=ht->func(key);
    while(1)
    {
        //可插入位置可以插入
        if(ht->data[offset].stat!=Valid)
        {
            ht->data[offset].key=key;
            ht->data[offset].value=value;
            ht->data[offset].stat=Valid;
            ht->size++;
            return;
        }
        //存在重复元素，插入失败
        else if(ht->data[offset].key==key&&ht->data[offset].stat==Valid)
        {
            return;
        }
        else
        {
            offset++;
            if(offset>=MaxSize)
            {
                offset=0;
            }
        }
    }
}
int HashFind(HashTable *ht,Keytype key,Valtype* value)
{
    if(ht==NULL)
    {
        return 0;
    }
    if(ht->size==0)
    {
        return 0;
    }
    size_t offset=ht->func(key);
    while(1)
    {
        //查找成功
        if(ht->data[offset].key==key&&ht->data[offset].stat==Valid)
        {
            *value=ht->data[offset].value;
            return 1;
        }
        //查找失败
        else if(ht->data[offset].stat==Empty)
        {
            return 0;
        }
        else
        {
            offset++;
            if(offset>=MaxSize)
            {
                offset=0;
            }
        }
    }
    return 0;
}

void HashRemove(HashTable *ht,Keytype key)
{
    if(ht==NULL)
    {
        return;
    }
    if(ht->size==0)
    {
        return;
    }
    size_t offset=ht->func(key);
    while(1)
    {
        if(ht->data[offset].key==key&&ht->data[offset].stat==Valid)
        {
            ht->data[offset].stat=Empty;
            --ht->size;
        }
        else if(ht->data[offset].stat==Empty)
        {
            return;
        }
        else
        {
            offset++;
            if(offset>=MaxSize)
            {
                offset=0;
            }
        }
    }
}
//////////////////////////////////////////////
//测试代码
/////////////////////////////////////////////
void TestInit()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht);
    printf("size expect 0,actual %lu\n",ht.size);
    printf("func expect %p,actual %p\n",HashDefault,ht.func);
    HashDestroy(&ht);
    printf("size expect 0,actual %lu\n",ht.size);
    printf("func expect NULL,actual %p\n",ht.func);
}
void HashPrint(HashTable* ht,const char* msg)
{
    if(ht==NULL)
    {
        return;
    }
    printf("%s\n",msg);
    size_t i=0;
    for(i=0;i<MaxSize;i++)
    {
        if(ht->data[i].stat==Empty)
        {
            continue;
        }
        printf("[%lu:%d %d]",i,ht->data[i].key,ht->data[i].value);
    }
    printf("\n");
}
void TestInsert()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1001,2000);
    HashInsert(&ht,2,3000);
    HashInsert(&ht,1002,4000);
    HashPrint(&ht,"插入若干个元素");
}
void TestFind()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1001,2000);
    HashInsert(&ht,2,3000);
    HashInsert(&ht,1002,4000);
    HashPrint(&ht,"插入若干个元素");
    Valtype value;
    int ret=HashFind(&ht,1,&value);
    printf("ret expect 1,actual %d\n",ret);
    printf("value expect 1000,actual %d\n",value);
    ret=HashFind(&ht,3,&value);
    printf("ret expect 0,actual %d\n",ret);

}
void TestRemove()
{
    TEST_HEADER;
    HashTable ht;
    HashInit(&ht);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1,1000);
    HashInsert(&ht,1001,2000);
    HashInsert(&ht,2,3000);
    HashInsert(&ht,1002,4000);
    HashPrint(&ht,"插入若干个元素");
    HashRemove(&ht,1);
    HashPrint(&ht,"删除一个元素");
}
int main()
{
    TestInit();
    TestInsert();
    TestFind();
    TestRemove();
    return 0;
}
