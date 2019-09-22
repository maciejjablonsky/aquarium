//
// Created by maciek on 02.07.19.
//

#include "sl_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct node node_t;

struct node
{
    void *item;
    node_t *next;
};


struct sl_list
{
    node_t *head;
    node_t *tail;
    size_t size; // number of items on the list
    size_t item_size;

    void *(*item_destructor)(void *item_to_delete);
};

node_t *sl_list_create_node(void *item, size_t item_size, copy_type copy);

node_t *sl_list_get_node(const sl_list_t *list, size_t index);

sl_list_t *SL_LIST_create(size_t item_size, void *(*item_destructor)(void *item_to_delete))
{
    sl_list_t *new_list = malloc(sizeof(sl_list_t));
    if (new_list == NULL)
    {
        return new_list;
    }
    new_list->head = new_list->tail = NULL;
    new_list->size = 0;
    new_list->item_size = item_size;
    new_list->item_destructor = item_destructor;
    return new_list;
}


node_t *sl_list_create_node(void *item, size_t item_size, copy_type copy)
{
    node_t *new_node = malloc(sizeof(node_t));
    if (new_node == NULL)
    {
        return new_node;
    }
    switch (copy)
    {
        case COPY_POINTER:new_node->item = item;
            break;
        case COPY_ITEM:
        default:new_node->item = malloc(item_size);
            if (new_node->item == NULL)
            {
                exit(1);
            }
            memcpy(new_node->item, item, item_size);
            break;
    }

    new_node->next = NULL;
    return new_node;
}


size_t SL_LIST_size(const sl_list_t *const list)
{
    if (list != NULL)
    {
        return list->size;
    }
    else
    {
        return 0;
    }
}


bool SL_LIST_add_item(sl_list_t *list, void *item, copy_type copy)
{
    node_t *new_node = sl_list_create_node(item, list->item_size, copy);
    if (new_node == NULL) { return false; }

    if (list->head == NULL)
    {
        list->head = new_node;
        list->tail = new_node;
    }
    else
    {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return true;
}


void *SL_LIST_item_at(const sl_list_t *const list, const size_t index)
{
    if (index >= list->size) { return NULL; }

    return sl_list_get_node(list, index)->item;
}


node_t *sl_list_get_node(const sl_list_t *const list, const size_t index)
{
    node_t *node = list->head;
    for (size_t i = 0; i < index; ++i)
    {
        node = node->next;
    }
    return node;
}


bool SL_LIST_apply_foreach(sl_list_t *list, void *(*func)(void *))
{
    if (list == NULL || list->head == NULL || func == NULL) { return false; }

    node_t *node = list->head;
    while (node != NULL)
    {
        node->item = (*func)(node->item);
        node = node->next;
    }
    return true;
}


bool SL_LIST_delete_item_at(sl_list_t *list, size_t index)
{
    if (index >= list->size) { return false; }

    if (index > 0)
    {
        node_t *prev_node = sl_list_get_node(list, index - 1);
        node_t *node_to_delete = prev_node->next;
        prev_node->next = prev_node->next->next;
        if (list->item_destructor != NULL)
        {
            list->item_destructor(node_to_delete->item);
        }
        else
        {
            free(node_to_delete->item);
        }
        free(node_to_delete);
    }
    else
    {
        node_t *node_to_delete = list->head;
        list->head = list->head->next;
        if (list->item_destructor != NULL)
        {
            list->item_destructor(node_to_delete->item);
        }
        else
        {
            free(node_to_delete->item);
        }
        free(node_to_delete);
    }
    list->size--;
    return true;
}

sl_list_t *SL_LIST_delete_list(sl_list_t *list)
{
    if (list != NULL)
    {
        while (list->size != 0)
        {
            if (!SL_LIST_delete_item_at(list, 0))
            {
                return list;
            }
        }
        free(list);
    }
    return NULL;
}
