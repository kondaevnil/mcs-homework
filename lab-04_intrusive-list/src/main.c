#include "clist.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct {
    intrusive_node node;
    int x, y;
} point;

void add_point(intrusive_list *list, int x, int y)
{
    point *new_point = (point *)malloc(sizeof(point));

    new_point->x = x;
    new_point->y = y;

    add_node(list, (intrusive_node *)new_point);
}

void remove_point(intrusive_list *list, int x, int y)
{
    intrusive_node *walker = list->head;
    point *tmp;

    while (walker->next != NULL)
    {
        if (((point *)walker)->x == x && ((point *)walker)->y == y)
        {
            remove_node(list, walker);
            tmp = (point *)walker;
            walker = walker->next;
            free(tmp);
        }
        else
            walker = walker->next;
    }
}

void show_all_points(intrusive_list *list)
{
    intrusive_node *walker = list->head;
    size_t counter = list->length;

    while (walker->next != NULL)
    {
        printf("(%d %d)%c", ((point *)walker)->x, ((point *)walker)->y, counter-- > 1 ? ' ' : '\n');

        walker = walker->next;
    }

    if (list->length == 0)
        printf("\n");
}

void remove_all_points(intrusive_list *list)
{
    intrusive_node *walker = list->head;
    point *tmp;

    while (walker->next != NULL)
    {
        remove_node(list, walker);
        tmp = (point *)walker;
        walker = walker->next;
        free(tmp);
    }
}

int main()
{
    char buf[1000];
    int exit = 0;
    int x, y;

    intrusive_list l;
    init_list(&l);

    while (!exit)
    {
        scanf("%999s", buf);

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
    free(l.head);
    return 0;
}
