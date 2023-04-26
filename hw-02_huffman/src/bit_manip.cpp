#include "bit_manip.h"
#include <algorithm>

hw2::bit_manip::bit_writer::bit_writer() : byte_buffer(std::vector<std::uint8_t>())
{}

void hw2::bit_manip::bit_writer::write_bits(const std::string &s_bits)
{
    for (auto ch: s_bits)
    {
        bit_buffer[bit_position++] = ch - '0';

        if (bit_position == constants::bits_in_byte)
        {
            byte_buffer.push_back(static_cast<std::uint8_t>(bit_buffer.to_ulong()));
            bit_position = 0;
        }
    }
}

const std::vector<std::uint8_t> & hw2::bit_manip::bit_writer::get_bytes() const
{
    return byte_buffer;
}

std::uint8_t hw2::bit_manip::bit_writer::get_extra_bits_count() const
{
    return (constants::bits_in_byte - bit_position) % 8;
}

void hw2::bit_manip::bit_writer::flush()
{
    if (bit_position != 0)
    {
        byte_buffer.push_back(static_cast<std::uint8_t>(bit_buffer.to_ulong()));
        bit_position = 0;
    }
}
