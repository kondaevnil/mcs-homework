#include "clist.h"

void init_list(intrusive_list *list)
{
    list->head = NULL;
    list->length = 0;
}

void add_node(intrusive_list *list, intrusive_node *node)
{
    if (list->head == NULL)
    {
        list->head = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        list->head->prev = node;
        node->next = list->head;
        node->prev = NULL;
        list->head = node;
    }

    list->length++;
}

void remove_node(intrusive_list *list, intrusive_node *node)
{
    if (node == list->head)
    {
        list->head = list->head->next;

        if (list->head != NULL)
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