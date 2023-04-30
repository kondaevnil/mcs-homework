#include "test_str.h"
#include "str.h"

#include <assert.h>
#include <string.h>
#include <stdio.h>

void test_name(const char *name)
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
    int number = 1;

    test_name("strcpy");

    test_number(number++);
    assert(my_strcpy(dest, src) == dest);
    strcpy(tmp_dest, src);
    assert(strcmp(dest, tmp_dest) == 0);


    test_number(number);
    assert(my_strcpy(dest, src) == dest);
    strcpy(tmp_dest + 5, src + 5);
    assert(strcmp(dest, tmp_dest) == 0);
}

void test_strcat(void)
{
    char dest[18];
    const char *result = "i love C language";
    int number = 1;

    test_name("strcat");

    test_number(number++);
    strcpy(dest, "i love");
    assert(my_strcat(dest, " C language") == dest);
    assert(strcmp(dest, result) == 0);

    test_number(number);
    memset(dest, 0, sizeof(dest));

    assert(my_strcpy(dest, "i love C language") == dest);
    assert(strcmp(dest, result) == 0);
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