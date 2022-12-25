#ifndef BMP_H
#define BMP_H

typedef struct
{
#pragma pack(push, 1)
    struct
    {
        unsigned int type : 16;
        unsigned int size;
        unsigned int reserved1 : 16;
        unsigned int reserved2 : 16;
        unsigned int offset;
    } file_header;
#pragma pack(pop)

#pragma pack(push, 1)
    struct
    {
        unsigned int size;
        int width;
        int height;
        unsigned int planes : 16;
        unsigned int bit_count : 16;
        unsigned int compression;
        unsigned int size_image;
        int x_pels_per_meter;
        int y_pels_per_meter;
        unsigned int clr_used;
        unsigned int clr_important;
    } info_header;
#pragma pack(pop)

    unsigned char *bytes;
} bmp_file;

int load_bmp(bmp_file *bmp, char *filename);

int crop(bmp_file *in_bmp, int x, int y, int w, int h);

int rotate(bmp_file *bmp);

int save_bmp(bmp_file *bmp, char *filename);

void close_bmp(bmp_file *bmp);

#define SUCCESS            0
#define READING_ERROR      1
#define BMP_FORMAT_ERROR   2
#define BIT_COUNT_ERROR    3
#define MEMORY_ALLOC_ERROR 4
#define FILE_OPEN_ERROR    5
#define NE_ARGS_ERROR      6
#define BOARDER_ERROR      7
#define OPERATION_ERROR    8
#define INSERT_ERROR       9
#define EXTRACT_ERROR     10

#endif