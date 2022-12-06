#ifndef POINT_LIST_H_
#define POINT_LIST_H_

#include "clist.h"

typedef struct {
    intrusive_node node;
    int x, y;
} point;

void add_point(intrusive_list *list, int x, int y);

void remove_all_points(intrusive_list *list);

void print_point(intrusive_node *node, void *data);

void count_points(intrusive_node *node, void *data);

void to_text_point(intrusive_node *node, void *data);

void to_bin_point(intrusive_node *node, void *data);

#endif