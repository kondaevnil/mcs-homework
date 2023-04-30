#include "bin_manip.h"

#include <utility>
#include <ostream>
#include <istream>

namespace manip
{
    write_str_s write_str(std::string s)
    {
        return write_str_s{std::move(s)};
    }

    write_bool_s write_bool(bool b)
    {
        return write_bool_s{b};
    }

    write_le_int32_s write_le_int32(int32_t x)
    {
        return write_le_int32_s{x};
    }

    read_bool_s read_bool(bool &b)
    {
        return read_bool_s{&b};
    }

    read_str_s read_str(std::string &str)
    {
        return read_str_s{&str};
    }

    read_le_int32_s read_le_int32(int32_t &x)
    {
        return read_le_int32_s{&x};
    }

    std::ostream & operator<<(std::ostream &os, write_le_int32_s wli)
    {
        for (int i = 0; i < sizeof(wli.x); i++)
            os.put(wli.x >> (8 * i) & 0xff);

        return os;
    }

    std::ostream & operator<<(std::ostream &os, const write_str_s& ws)
    {
        const char *beg = ws.s.c_str();
        int i = 0;

        do
        {
            os << *(beg + i);
        } while (*(beg + i++) != '\0');

        return os;
    }

    std::ostream & operator<<(std::ostream &os, write_bool_s wb)
    {
        os.put(wb.b);

        return os;
    }

    std::istream & operator>>(std::istream &is, read_bool_s rb)
    {
        char ch;

        if (!is.get(ch))
            throw std::invalid_argument("File doesn't contain enough data.");

        *rb.b = ch & 1;

        return is;
    }
    std::istream & operator>>(std::istream &is, read_le_int32_s rli)
    {
        int32_t x = 0;
        char ch = 0;

        for (int i = 0; i < 4; i++)
        {
            if (!is.get(ch))
                throw std::invalid_argument("File doesn't contain enough data.");

            x |= (ch & 0xFF) << (8 * i);
        }

        *rli.x = x;

        return is;
    }
    std::istream & operator>>(std::istream &is, read_str_s rs)
    {
        char ch;

        is.get(ch);
        while (ch != '\0')
        {
            *rs.s += ch;

            if (!is.get(ch))
                throw std::invalid_argument("File doesn't contain enough data.");
        }

        return is;
    }
}