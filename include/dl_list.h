//
// Created by maciek on 01.10.19.
//

#ifndef DOUBLE_LINKED_LIST_DL_LIST_H
#define DOUBLE_LINKED_LIST_DL_LIST_H

#include <stdbool.h>
#include <stddef.h>

typedef enum
{
    DL_COPY_ITEM, DL_COPY_POINTER
} dl_storage_type;

typedef struct dl_node dl_node_t;
struct dl_node
{
    void *item;
    dl_node_t *next;
    dl_node_t *prev;
};

typedef struct
{
    dl_node_t *node;
    size_t i;
} dl_iterator_t;

typedef struct
{
    dl_node_t *head;
    dl_node_t *tail;
    size_t size;
    size_t item_size;
    dl_storage_type storage_info;
    void *(*item_destructor)(void *item_to_delete);

    dl_iterator_t current;
} dl_list_t;


/*
 * creates initialized double linked list
 *
 * return value: address of initialized dl_list or NULL in case of memory error
 * parameters:
 *  - item_size: size of item in bytes
 *  - storage_info: storage_type enum describing type of item on the list (copy of item or pointer to item)
 *  - destructor: function handling deleting item, might be NULL when item doesn't have dynamically allocated memory
 */
dl_list_t *DL_LIST_create(size_t item_size, dl_storage_type storage_info, void *(*destructor)(void *item_to_delete));

/*
 * adds item at the end of the list
 *
 * return value: returns true if everything went fine, otherwise false (doesn't add item then)
 * parameters:
 *  - list: initialized dl_list
 *  - item: address of item to copy
 */
bool DL_LIST_add_item(dl_list_t *list, void *item);

/*
 * gets number of items on the list
 *
 * return value: size_t number of items
 * parameters:
 *  - initialized dl_list
 */
size_t DL_LIST_size(const dl_list_t *list);


/*
 * deletes item at index position
 *
 * return value: true if item deleted, false otherwise (e.g. index is too big)
 * parameters:
 *  - initialized dl_list
 *  - index of item to delete
 */
bool DL_LIST_delete_item_at(dl_list_t *list, size_t index);


/*
 * returns address of item at 'index'
 *
 * return value: address of item, or NULL in case of too big index
 * parameters:
 *  - initialized dl_list
 *  - size_t index of item
 */
void *DL_LIST_item_at(dl_list_t *list, size_t index);


/*
 * deletes whole list
 *
 * return value: NULL, or some pointer to list if error occurred
 * parameters:
 *  - initialized list
 *
 *  example usage:
 *
 *  list = DL_LIST_delete(list);
 */
dl_list_t *DL_LIST_delete(dl_list_t *list);


/*
 * loop that goes through whole list
 *
 * parameters:
 *  - dl_list_ptr: pointer to initialized dl_list_t
 *  - item: pointer to item_type
 *
 * example:
 * int * item = NULL;
 * dl_list_foreach(list, item)
 * {
 *      (*item)++;
 * }
 */
#define dl_list_foreach(dl_list_ptr, own_item) \
    for (\
        dl_node_t *cursor_node_ptr = (dl_list_ptr)->head;\
        (cursor_node_ptr)\
        && ((own_item) = (cursor_node_ptr)->item);\
        cursor_node_ptr = cursor_node_ptr->next\
    )\


#endif //DOUBLE_LINKED_LIST_DL_LIST_H
