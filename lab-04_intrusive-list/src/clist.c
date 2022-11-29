#include "clist.h"
#include <stdlib.h>

void init_list(intrusive_list *list)
{
    list->head = (intrusive_node *)malloc(sizeof(intrusive_node));
    list->length = 0;
}

void add_node(intrusive_list *list, intrusive_node *node)
{
    list->head->prev = node; // append new node before head
    node->next = list->head; // append head to next element of node
    list->head = node;       // swap head and new node

    list->length++;          // increase length
}

void remove_node(intrusive_list *list, intrusive_node *node)
{
    if (node == list->head)
        list->head = node->next; // if node is head then move head to the next node
    else
    {
        node->prev->next = node->next;
        node->next->prev = node->prev;
    }

    list->length--;
}

size_t get_length(intrusive_list *list)
{
    return list->length;
}