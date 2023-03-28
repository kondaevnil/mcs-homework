#ifndef LAB10_BIN_MANIP_H_INCLUDED
#define LAB10_BIN_MANIP_H_INCLUDED

#include <cstdint>
#include <string>

namespace manip
{
    struct write_le_int32_s
    {
        int32_t x;
    };

    struct write_bool_s
    {
        bool b;
    };

    struct write_str_s
    {
        std::string s;
    };

    struct read_bool_s
    {
        bool *b;
    };

    struct read_str_s
    {
        std::string *s;
    };

    struct read_le_int32_s
    {
        int32_t *x;
    };

    std::ostream & operator<<(std::ostream &, write_le_int32_s);
    std::ostream & operator<<(std::ostream &, const write_str_s &);
    std::ostream & operator<<(std::ostream &, write_bool_s);

    std::istream & operator>>(std::istream &, read_bool_s);
    std::istream & operator>>(std::istream &, read_le_int32_s);
    std::istream & operator>>(std::istream &, read_str_s);

    write_str_s write_str(std::string s);
    write_bool_s write_bool(bool b);
    write_le_int32_s write_le_int32(int32_t x);

    read_str_s read_str(std::string &str);
    read_le_int32_s read_le_int32(int32_t &x);
    read_bool_s read_bool(bool &b);
}

#endif