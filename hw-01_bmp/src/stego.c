#include "stego.h"
#include "stdio.h"

const unsigned char set_mask = 0xfe;
const unsigned char get_mask = 0x01;
const int scan_required = 3;
const int color_count = 3;
const int bits_count = 5;

static unsigned char char_to_bits(char ch)
{
    switch (ch)
    {
        case ' ':
            return 26;

        case '.':
            return 27;

        case ',':
            return 28;

        default:
            return ch - 'A';
    }
}

static char bits_to_char(unsigned char bits)
{
    switch (bits)
    {
        case 26:
            return ' ';

        case 27:
            return '.';

        case 28:
            return ',';

        default:
            return bits + 'A';
    }
}

static int check_bits(unsigned char bits)
{
    return bits <= 28;
}

static int check_char(char ch)
{
    return (('A' <= ch && ch <= 'Z') || ch == ' ' || ch == ',' || ch == '.');
}

static int get_color_pos(char color)
{
    if (color == 'B')
        return 0;
    else if (color == 'G')
        return 1;
    else if (color == 'R')
        return 2;
    else
        return -1;
}

static int insert_bit(bmp_file *bmp, int x, int y, char color, unsigned char bit)
{
    int color_pos = get_color_pos(color);

    if (x < 0 || bmp->info_header.width <= x || y < 0 || bmp->info_header.height <= y || color == -1)
        return 0;

    bmp->bytes[y * bmp->info_header.width * color_count + x * color_count + color_pos] &= set_mask;
    bmp->bytes[y * bmp->info_header.width * color_count + x * color_count + color_pos] |= bit;

    return 1;
}

static int extract_bit(bmp_file *bmp, int x, int y, char color, unsigned char *bit)
{
    int color_pos = get_color_pos(color);

    if (x < 0 || bmp->info_header.width <= x || y < 0 || bmp->info_header.height <= y || color == -1)
        return 0;

    *bit = bmp->bytes[y * bmp->info_header.width * color_count + x * color_count + color_pos] & get_mask;

    return 1;
}

int insert_message(bmp_file *bmp, const char *key_filename, const char *message_filename)
{
    FILE *message_file;
    FILE *key_file;

    if ((message_file = fopen(message_filename, "r")) == NULL)
        return FILE_OPEN_ERROR;

    if ((key_file = fopen(key_filename, "r")) == NULL)
        return FILE_OPEN_ERROR;

    int x_pos, y_pos;
    char color;
    char ch;

    while ((ch = fgetc(message_file)) && ch != '\n')
    {
        unsigned char bits = char_to_bits(ch);

        for (int i = 0; i < bits_count; i++)
        {
            int scan_result = fscanf(key_file, "%d%d %c", &x_pos, &y_pos, &color);

            if (scan_result != scan_required)
            {
                fclose(message_file);
                fclose(key_file);

                return READING_ERROR;
            }

            int insert_ok = insert_bit(bmp, x_pos, y_pos, color, (bits >> i) * get_mask);

            if (!insert_ok)
            {
                fclose(message_file);
                fclose(key_file);

                return INSERT_ERROR;
            }
        }
    }

    fclose(message_file);
    fclose(key_file);

    return SUCCESS;
}

int extract_message(bmp_file *bmp, const char *key_filename, const char *message_filename)
{
    FILE *message_file;
    FILE *key_file;

    if ((message_file = fopen(message_filename, "w")) == NULL)
        return FILE_OPEN_ERROR;

    if ((key_file = fopen(key_filename, "r")) == NULL)
        return FILE_OPEN_ERROR;

    int x_pos, y_pos;
    char color;
    unsigned char bits = 0;
    unsigned char bit;
    int count = 0;

    while (fscanf(key_file, "%d%d %c", &x_pos, &y_pos, &color) == scan_required)
    {
        int extract_ok = extract_bit(bmp, x_pos, y_pos, color, &bit);

        if (!extract_ok)
        {
            fclose(message_file);
            fclose(key_file);

            return EXTRACT_ERROR;
        }

        bits = bits | (bit << count);
        count++;

        if (count % 5 == 0)
        {
            fprintf(message_file, "%c", bits_to_char(bits));
            count = 0;
        }
    }

    fclose(message_file);
    fclose(key_file);

    return SUCCESS;
}
