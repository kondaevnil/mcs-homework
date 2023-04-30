#### 1. Multiple definition
Ошибка возникает при повторном определениии функции, например:
io.c:
    ...
    void logDebug(char ch)
    {
        printf("%c", ch);
    }

    void logDebug(char ch)
    {
        printf("info: %c\n", ch);
    }


#### 2. Undefined reference
Данная ошибка возникает, когда функция была объявлена и использована, но при этом нигде не определена. Ошибка выбрасывается на этапе линковки, когда линкер не находит реализацию функции ни в одном из *.o файлов.