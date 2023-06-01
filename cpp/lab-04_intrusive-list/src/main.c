#include "clist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct {
    intrusive_node node;
    int x, y;
} point;

void add_point(intrusive_list *list, int x, int y)
{
    point *new_pt = (point *)malloc(sizeof(point));

    new_pt->x = x;
    new_pt->y = y;

    add_node(list, (intrusive_node *)new_pt);
}

void remove_point(intrusive_list *list, int x, int y)
{
    point *pt;
    intrusive_node *walker = list->head;

    while (walker != NULL)
    {
        pt = (point *)walker;
        walker = walker->next;

        if (pt->x == x && pt->y == y)
        {
            remove_node(list, (intrusive_node *)pt);
            free(pt);
        }
    }
}

void show_all_points(intrusive_list *list)
{
    if (list->length == 0)
    {
        printf("\n");
        return;
    }

    point *walker = (point *)list->head;
    size_t counter = list->length;

    while (walker != NULL)
    {
        printf("(%d %d)%c", walker->x, walker->y, counter > 1 ? ' ' : '\n');

        walker = (point *)walker->node.next;
        counter--;
    }
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

int main()
{
    char buf[100];
    int x, y;
    int exit = 0;
    intrusive_list l;

    init_list(&l);

    while (!exit)
    {
        scanf("%99s", buf);

        if (strcmp(buf, "rm") == 0)
        {
            scanf("%d%d", &x, &y);
            remove_point(&l, x, y);
        }
        else if (strcmp(buf, "add") == 0)
        {
            scanf("%d%d", &x, &y);
            add_point(&l, x, y);
        }
        else if (strcmp(buf, "print") == 0)
            show_all_points(&l);
        else if (strcmp(buf, "rma") == 0)
            remove_all_points(&l);
        else if (strcmp(buf, "exit") == 0)
            exit = 1;
        else if (strcmp(buf, "len") == 0)
            printf("%zu\n", get_length(&l));
        else
            printf("Unknown command\n");
    }


    remove_all_points(&l);
    return 0;
}
