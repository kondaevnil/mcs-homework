#include "point_list.h"
#include <stdio.h>
#include <string.h>

#define MAX_SIZE 100

void read_text(FILE *F, intrusive_list *list)
{
    char buf[MAX_SIZE];
    int x, y;

    while (fgets(buf, MAX_SIZE, F) != NULL)
    {
        sscanf(buf, "%d%d", &x, &y);
        add_point(list, x, y);
    }
}

#pragma pack(push, 2)
typedef struct
{
    int x : 24;
    int y : 24;
} bin_point;
#pragma pack(pop)

void read_bin(FILE *F, intrusive_list *list)
{
    bin_point pt = {};

    while (fread(&pt, sizeof(bin_point), 1, F) != 0)
        add_point(list, pt.x, pt.y);
}

typedef enum
{
    MODE_TEXT,
    MODE_BINARY,
} mode;

#define CONCAT(A, B) A ## B

#define STRINGIZE_NOT_EXPAND(A) #A

#define STRINGIZE(A) STRINGIZE_NOT_EXPAND(A)

#define DETECT_MODE(operation) \
        strcmp(operation##_mode, STRINGIZE(CONCAT(operation, text))) == 0 ? MODE_TEXT  : \
        strcmp(operation##_mode, STRINGIZE(CONCAT(operation, bin))) == 0 ? MODE_BINARY : \
        -1

int read_points(intrusive_list *list, char *load_mode, char *file_name)
{
    FILE *F;

    void (*read[])(FILE *F, intrusive_list *list) = {read_text, read_bin};

    int mode = DETECT_MODE(load);

    if (mode == -1 || (F = fopen(file_name, mode == MODE_TEXT ? "r" : "rb")) == NULL)
        return 0;

    read[mode](F, list);

    fclose(F);
    return 1;
}

int save_points(intrusive_list *list, char *save_mode, char *file_name)
{
    FILE *F;

    void (*write[])(intrusive_node *node, void *data) = {to_text_point, to_bin_point};

    int mode = DETECT_MODE(save);

    if (mode == -1 || (F = fopen(file_name, mode == MODE_TEXT ? "w" : "wb")) == NULL)
        return 0;

    apply(list, write[mode], F);

    fclose(F);
    return 1;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
        return 0;

    int params_counter = 1;

    intrusive_list list;
    init_list(&list);

    if (!read_points(&list, argv[params_counter], argv[params_counter + 1]))
        return 0;

    params_counter += 2;

    while (params_counter < argc)
    {
        if (strcmp(argv[params_counter], "print") == 0)
        {
            params_counter++;

            apply(&list, print_point, argv[params_counter++]);
            printf("\n");
        }
        else if (strcmp(argv[params_counter], "count") == 0)
        {
            params_counter++;

            int count = 0;
            apply(&list, count_points, &count);

            printf("%d\n", count);
        }
        else if (save_points(&list, argv[params_counter], argv[params_counter + 1]))
            params_counter += 2;
    }

    remove_all_points(&list);
}