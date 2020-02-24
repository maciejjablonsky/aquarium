#include "dl_list.h"
#include "object.h"
#include "memory_handling.h"
#include "exit_codes.h"
#include <stdlib.h>
#include <memory.h>
#include <assert.h>

dl_node_t *dl_list_create_node(void *item, size_t item_size, dl_storage_type info);

// unguarded
dl_node_t *dl_list_get_node(dl_list_t *this, size_t dest_index);

void dl_list_delete_node(dl_list_t *this, dl_node_t *node_to_delete);

dl_list_t *new_DL_LIST(size_t item_size, dl_storage_type storage_info, void *(*destructor)(void *)) {
    dl_list_t *this = new_object(sizeof(dl_list_t));
    if (is_not_created(this)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    this->head = this->tail = NULL;
    this->item_size = item_size;
    this->storage_info = storage_info;
    this->item_destructor = destructor;
    this->current.node = NULL;
    this->current.i = 0;
    this->size = 0;
    return this;
}

bool DL_LIST_add_item(dl_list_t *const this, void *const item) {
    assert(is_created(this));

    dl_node_t *new_node = dl_list_create_node(item, this->item_size, this->storage_info);
    if (is_not_created(new_node)) {
        NEW_OBJECT_FAILURE(DL_LIST_T_NAME);
        return false;
    }

    if (is_not_created(this->head)) {
        this->head = new_node;
        this->tail = new_node;
        this->current.node = new_node;
    } else {
        new_node->prev = this->tail;
        this->tail->next = new_node;
        this->tail = new_node;
    }
    this->size++;
    return true;
}

dl_node_t *dl_list_create_node(void *const item, const size_t item_size, const dl_storage_type info) {
    dl_node_t *new_node = new_object(sizeof(dl_node_t));
    if (is_not_created(new_node)) {
        MEMORY_NOT_ALLOCATED_MESSAGE();
        return NULL;
    }
    switch (info) {
        case DL_COPY_POINTER:
            new_node->item = item;
            break;
        case DL_COPY_ITEM:
        default:
            new_node->item = malloc(item_size);
            if (is_created(new_node->item)) {
                memcpy(new_node->item, item, item_size);
            } else {
                exit(EXIT_NO_MEMORY);
            }
    }
    new_node->next = new_node->prev = NULL;
    return new_node;
}


size_t DL_LIST_size(const dl_list_t *const this) {
    assert(is_created(this));
    return this->size;
}

dl_node_t *dl_list_get_node(dl_list_t *const this, const size_t dest_index) {
    dl_iterator_t *current = &this->current;

    if (current->i == dest_index) {
        return current->node;
    } else if (dest_index == 0) {
        current->node = this->head;
        current->i = 0;
    } else if (dest_index == this->size - 1) {
        current->node = this->tail;
        current->i = this->size - 1;
    } else if (current->i < dest_index) {
        size_t left = dest_index - current->i;
        size_t right = this->size - 1 - dest_index;
        if (left < right) {
            for (size_t i = 0; i < left; ++i) {
                current->node = current->node->next;
                ++current->i;
            }
        } else {
            current->node = this->tail;
            current->i = this->size - 1;
            for (size_t i = 0; i < right; ++i) {
                current->node = current->node->prev;
                --current->i;
            }
        }
    } else {
        size_t left = dest_index;
        size_t right = current->i - dest_index;
        if (left < right) {
            current->node = this->head;
            current->i = 0;
            for (size_t i = 0; i < left; ++i) {
                current->node = current->node->next;
                ++current->i;
            }
        } else {
            for (size_t i = 0; i < right; ++i) {
                current->node = current->node->prev;
                --current->i;
            }
        }
    }
    return current->node;
}

void *DL_LIST_item_at(dl_list_t *this, const size_t index) {
    if (index >= this->size) {
        IMPLICIT_ERROR_MESSAGE("Index is out of range.");
        return NULL;
    }
    return dl_list_get_node(this, index)->item;
}

bool DL_LIST_delete_item_at(dl_list_t *this, const size_t index) {
    if (index >= this->size) {
        IMPLICIT_ERROR_MESSAGE("Index is out of range.");
        return false;
    }

    dl_node_t *node_to_delete = dl_list_get_node(this, index);
    if (index < this->size - 1) {
        dl_list_get_node(this, index + 1);
        this->current.i--;
    } else {
        dl_list_get_node(this, index - 1);
    }

    if (this->item_destructor) {
        this->item_destructor(node_to_delete->item);
    } else {
        free(node_to_delete->item);
    }

    dl_list_delete_node(this, node_to_delete);
    return true;
}

void dl_list_delete_node(dl_list_t *this, dl_node_t *node_to_delete) {
    if (this->head == node_to_delete) {
        this->head = node_to_delete->next;
    }
    if (this->tail == node_to_delete) {
        this->tail = node_to_delete->prev;
    }

    if (node_to_delete->prev) {
        node_to_delete->prev->next = node_to_delete->next;
    }
    if (node_to_delete->next) {
        node_to_delete->next->prev = node_to_delete->prev;
    }

    free(node_to_delete);
    this->size--;
}

dl_list_t *delete_DL_LIST(dl_list_t *list) {
    if (is_created(list)) {
        while (list->size != 0) {
            if (!DL_LIST_delete_item_at(list, 0)) {
                DELETE_OBJECT_FAILURE(DL_LIST_T_NAME);
                exit(EXIT_FREE_MEMORY_ERROR);
            }
        }
        free(list);
    }
    return NULL;
}