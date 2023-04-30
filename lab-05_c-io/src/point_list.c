#include "point_list.h"
#include <stdio.h>
#include <stdlib.h>

void add_point(intrusive_list *list, int x, int y)
{
    point *new_pt = (point *)malloc(sizeof(point));

    new_pt->x = x;
    new_pt->y = y;

    add_node(list, (intrusive_node *)new_pt);
}

void remove_all_points(intrusive_list *list)
{
    point *pt;
    intrusive_node *walker = list->head;

    while (walker != NULL)
    {
        pt = (point *)walker;
        walker = walker->next;
        remove_node(list, (intrusive_node *)pt);
        free(pt);
    }
}

void print_point(intrusive_node *node, void *data)
{
    point *pt = (point *)node;

    printf((char *)data, pt->x, pt->y);
}

void count_points(intrusive_node *node, void *data)
{
    (*(size_t *)data)++;
}

void to_text_point(intrusive_node *node, void *data)
{
    FILE *F = (FILE *)data;
    point *pt = (point *)node;

    fprintf(F, "%d %d\n", pt->x, pt->y);
}

void to_bin_point(intrusive_node *node, void *data)
{
    FILE *F = (FILE *)data;
    point *pt = (point *)node;

    fwrite(&pt->x, 3, 1, F);
    fwrite(&pt->y, 3, 1, F);
}
