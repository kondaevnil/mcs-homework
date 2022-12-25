#ifndef STEGO_H
#define STEGO_H

#include "bmp.h"

int insert_message(bmp_file *bmp, const char *key_filename, const char *message_filename);

int extract_message(bmp_file *bmp, const char *key_filename, const char *message_filename);

#endif