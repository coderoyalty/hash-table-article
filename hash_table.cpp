#include "hash_table.hpp"

HashNode *initHashNode(const char *key, const char *value)
{
    HashNode *hNode = NULL;
    // a key cannot be empty.
    if (key == NULL || strlen(key) == 0 || value == NULL)
        return (NULL);
    hNode = (HashNode *)malloc(sizeof(struct HashNode));
    if (hNode == NULL)
        return (NULL);
    hNode->key = strdup(key);
    hNode->value = strdup(value);
    return (hNode);
}

HashMap *initHashMap(unsigned long size)
{
    HashMap *hMap;
    if (size <= 0)
        return (NULL);
    hMap = (HashMap *)malloc(sizeof(struct HashMap));
    if (!hMap)
        return (NULL);
    hMap->size = size;
    hMap->array = (struct HashNode **)malloc(sizeof(struct HashNode *) * size);
    if (hMap->array == NULL)
    {
        free(hMap);
        return (NULL);
    }
    for (int i = 0; i < size; i++)
        hMap->array[i] = NULL;
    return (hMap);
}

unsigned long int djb2(unsigned char *str)
{
    unsigned long int hash = 5381;
    int ch;
    while ((ch = *str++)) hash = hash * 33 + ch;
    return hash;
}

unsigned long int key_index(const char *key, unsigned long int size)
{
    unsigned long int hash = djb2((unsigned char *)key);
    return hash % size;
}

bool insert(HashMap *hmap, const char *key, const char *value)
{
    if (!hmap)
        return false;
    // create the a hash node
    struct HashNode *node = initHashNode(key, value);
    if (!node)
        return false;
    unsigned long int hIndex = key_index(key, hmap->size);
    if (hmap->array[hIndex] == NULL)
    {
        hmap->array[hIndex] = node;
        return true;
    }
    // if the key already exists I'll replace the value
    // by deleting it and duplicating the new value.
    if (strcmp(hmap->array[hIndex]->key, key) == 0)
    {
        free(node->key);
        free(node->value);
        free(node);
        node = NULL;
        node = hmap->array[hIndex];
        free(node->value);
        node->value = strdup(value);
    }
    return true;
}

void printMap(HashMap *hmap)
{
    if (!hmap)
        return;
    printf("{\n");
    for (int i = 0; i < hmap->size; i++)
    {
        struct HashNode *node = hmap->array[i];
        if (node)
            printf("\t\"%s\" : \"%s\"\n", node->key, node->value);
    }
    printf("}\n");
}