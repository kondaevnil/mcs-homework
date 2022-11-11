#include "test_str.h"
#include "str.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_name(char *name)
{
    printf("# %s test\n", name);
}

void test_number(int number)
{
    printf("  -- Test #%d\n", number);
}

void test_strcpy(void)
{
    char dest[12], tmp_dest[12];
    const char *src = "hello";
    size_t i;
    int number = 1;

    test_name("strcpy");

    test_number(number++);
    my_strcpy(dest, src);
    strcpy(tmp_dest, src);

    for (i = 0; i < strlen(dest) + 1; i++)
        assert(dest[i] == tmp_dest[i]);


    test_number(number);
    my_strcpy(dest + 5, src + 5);
    strcpy(tmp_dest + 5, src + 5);

    for (i = 0; i < strlen(dest) + 1; i++)
        assert(dest[i] == tmp_dest[i]);
}

void test_strcat(void)
{
    char dest[18];
    const char *result = "i love C language";
    int number = 1;
    size_t i;

    strcpy(dest, "i love");
    my_strcat(dest, " C language");

    test_name("strcat");

    test_number(number++);
    for (i = 0; i < strlen(dest) + 1; i++)
        assert(dest[i] == result[i]);

    test_number(number);
    for (i = 0; i < 18; i++)
        dest[i] = 0;

    my_strcpy(dest, "i love C language");
    for (i = 0; i < strlen(dest) + 1; i++)
        assert(dest[i] == result[i]);
}

void test_strcmp(void)
{
    int number = 1;

    test_name("strcmp");

    test_number(number++);
    assert(my_strcmp("pointer on", "pointer on int") < 0 && strcmp("pointer on", "pointer on int") < 0);

    test_number(number++);
    assert(my_strcmp("pointer on int", "pointer on") > 0 && strcmp("pointer on int", "pointer on") > 0);

    test_number(number++);
    assert(my_strcmp("pointer on int", "pointer on int") == strcmp("pointer on int", "pointer on int"));

    test_number(number);
    assert(my_strcmp("poInter", "pointer") < 0 && strcmp("poInter", "pointer") < 0);

    test_number(number);
    assert(my_strcmp("pointer", "poInter") > 0 && strcmp("pointer", "poInter") > 0);
}

void test_strlen(void)
{
    int number = 1;

    test_name("strlen");

    test_number(number++);
    assert(my_strlen("") == strlen(""));

    test_number(number);
    assert(my_strlen("#include <stdio.h>") == strlen("#include <stdio.h>"));
}