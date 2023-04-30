#ifndef HW2_BIT_MANIP_H
#define HW2_BIT_MANIP_H

#include <bitset>
#include <vector>
#include <ostream>
#include <cstddef>

namespace hw2
{
    namespace constants
    {
        const std::size_t bits_in_byte = 8;
    }

    namespace huffman
    {
        class archiver;
    }
    namespace bit_manip
    {
        class bit_writer
        {
            friend huffman::archiver;

            std::vector<std::uint8_t> byte_buffer;
            std::bitset<8> bit_buffer{};
            std::uint8_t bit_position = 0;

            bit_writer();
            void write_bits(const std::string &s_bits);
            [[nodiscard]] const std::vector<std::uint8_t> & get_bytes() const;
            [[nodiscard]] std::uint8_t get_extra_bits_count() const;
            void flush();
        };
    }
}

#endif // HW2_BIT_MANIP_H