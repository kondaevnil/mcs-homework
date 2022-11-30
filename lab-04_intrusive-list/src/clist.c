#include "clist.h"
#include <stdlib.h>

void init_list(intrusive_list *list)
{
    list->head = (intrusive_node *)malloc(sizeof(intrusive_node));
    list->length = 0;
}

void add_node(intrusive_list *list, intrusive_node *node)
{
    list->head->prev = node;
    node->next = list->head;
    node->prev = NULL;
    list->head = node;

    list->length++;
}

void remove_node(intrusive_list *list, intrusive_node *node)
{
    if (node == list->head)
    {
        list->head = list->head->next;
        list->head->prev = NULL;
    }
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