#include "clist.h"

void init_list(intrusive_list *list)
{
    list->head = NULL;
    list->tail = NULL;
}

void add_node(intrusive_list *list, intrusive_node *node)
{
    if (list->head == NULL)
    {
        list->head = node;
        list->tail = node;
        node->next = NULL;
        node->prev = NULL;
    }
    else
    {
        list->tail->next = node;
        node->prev = list->tail;
        node->next = NULL;
        list->tail = node;
    }
}

void remove_node(intrusive_list *list, intrusive_node *node)
{
    if (node == list->head)
    {
        list->head = list->head->next;

        if (list->head != NULL)
            list->head->prev = NULL;
    }
    else if (node == list->tail)
    {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }
    else
    {
        node->prev->next = node->next;
        if (node->next != NULL)
            node->next->prev = node->prev;
    }
}

void apply(intrusive_list *list, void (*op)(intrusive_node *node, void *data), void *data)
{
    intrusive_node *walker = list->head;

    while (walker != NULL)
    {
        op(walker, data);
        walker = walker->next;
    }
}
