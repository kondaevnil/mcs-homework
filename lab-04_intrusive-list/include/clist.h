#ifndef CLIST_H_
#define CLIST_H_

#include <stddef.h>

typedef struct intrusive_node_ intrusive_node;

struct intrusive_node_ {
  intrusive_node *next;
  intrusive_node *prev;
};

typedef struct {
  intrusive_node *head;
  size_t length;
} intrusive_list;

void init_list(intrusive_list *list);
void add_node(intrusive_list *list, intrusive_node *node);
void remove_node(intrusive_list *list, intrusive_node *node);

size_t get_length(intrusive_list *list);

#endif
