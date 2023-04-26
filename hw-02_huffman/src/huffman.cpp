#include "huffman.h"

#include <utility>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include <iostream>

namespace hw2::huffman
{
    bool tree::node::operator>(const tree::node &other) const
    {
        return occurrence_ > other.occurrence_;
    }

    tree::node::node(std::pair<char, std::size_t> pr) : value_(pr.first), occurrence_(pr.second),
        left(nullptr), right(nullptr)
    {
    }

    tree::node::node(tree::nodeptr le, tree::nodeptr ri)
    {
        occurrence_ = le->occurrence_ + ri->occurrence_;
        left = std::move(le);
        right = std::move(ri);
    }

    occurrence_table::occurrence_table(const std::vector<char> &bytes)
    {
        for (auto byte: bytes)
            occurrence_[byte]++;
    }

    occurrence_table::occurrence_table(const std::map<char, std::size_t> &occ_map)
    {
        occurrence_ = occ_map;
    }


    tree::tree(const occurrence_table &table)
    {
        auto compare = [](const nodeptr& le, const nodeptr& ri)
        {
            return le->occurrence_ > ri->occurrence_;
        };
        std::priority_queue<nodeptr, std::vector<nodeptr>, decltype(compare)> pq(compare);

        for (const auto &pair: table.occurrence_)
            pq.emplace(new node(pair));

        while (pq.size() > 1)
        {
            nodeptr first = pq.top();
            pq.pop();
            nodeptr second = pq.top();
            pq.pop();

            pq.emplace(new node(std::move(first), std::move(second)));
        }

        if (!pq.empty())
        {
            root = pq.top();
            build_codes(root, "");
        }
    }

    void tree::build_codes(const nodeptr &current, const std::string  &code)
    {
        if (current == nullptr)
            return;

        if (current->left == nullptr && current->right == nullptr)
        {
            if (current == root)
            {
                char_code[current->value_] = "0";
                code_char["0"] = current->value_;
            }
            else
            {
                char_code[current->value_] = code;
                code_char[code] = current->value_;
            }
            return;
        }
        build_codes(current->left, code + "0");
        build_codes(current->right, code + "1");
    }

    std::string & tree::get_code(char ch)
    {
        return char_code[ch];
    }

    std::uint32_t tree::get_alphabet_size() const
    {
        return char_code.size();
    }

    const std::map<std::string, char> & tree::get_code_char() const
    {
        return code_char;
    }

    void archiver::archive(const std::string &in_file, const std::string &out_file)
    {
        std::ifstream in(in_file, std::ios::in | std::ios::binary);

        if (!in.is_open())
            throw std::invalid_argument("File doesn't exist.");

        char ch_in;
        in.read(&ch_in, 1);
        std::vector<char> data;

        while (!in.eof())
        {
            data.push_back(ch_in);
            in.read(&ch_in, 1);
        }
        in.close();

        occurrence_table occ_table(data);
        tree huff_tree(occ_table);

        std::ofstream out(out_file, std::ios::binary);
        bit_manip::bit_writer bw;

        for (char ch: data)
            bw.write_bits(huff_tree.get_code(ch));

        std::size_t additional_data_size = 0;
        auto alpha_size = huff_tree.get_alphabet_size();
        auto extra_bits = bw.get_extra_bits_count();
        out.write(reinterpret_cast<char *>(&alpha_size), sizeof(alpha_size));
        out.write(reinterpret_cast<char *>(&extra_bits), 1);

        additional_data_size += 1 + sizeof(alpha_size);

        for (auto &pr: occ_table.occurrence_)
        {
            out.write(&pr.first, 1);
            out.write(reinterpret_cast<char *>(&pr.second), sizeof(std::size_t));
            additional_data_size += 1 + sizeof(std::size_t);
        }

        bw.flush();
        auto bytes = bw.get_bytes();
        for (auto byte: bytes)
            out.write(reinterpret_cast<char *>(&byte), 1);

        std::cout << data.size() << std::endl;
        std::cout << bw.get_bytes().size() << std::endl;
        std::cout << additional_data_size << std::endl;

        out.close();
    }

    void archiver::unarchive(const std::string &in_file, const std::string &out_file)
    {
        std::ifstream in(in_file, std::ios::binary);

        if (!in.is_open())
            throw std::invalid_argument("File doesn't exist.");

        std::size_t additional_data_size = 0;

        std::uint32_t alphabet_size;
        in.read(reinterpret_cast<char *>(&alphabet_size), sizeof(alphabet_size));
        if (in.fail())
            throw std::invalid_argument("Incorrect header format");

        std::uint8_t extra_bits;
        in.read(reinterpret_cast<char *>(&extra_bits), 1);
        if (in.fail())
            throw std::invalid_argument("Incorrect header format");

        additional_data_size += sizeof(alphabet_size) + 1;

        std::map<char, std::size_t> occ_map;

        for (int i = 0; i < alphabet_size; i++)
        {
            char ch;
            in.read(&ch, 1);
            if (in.fail())
                throw std::invalid_argument("Incorrect header format");

            std::size_t occurrence;
            in.read(reinterpret_cast<char *>(&occurrence), sizeof(std::size_t));

            if (in.fail())
                throw std::invalid_argument("Incorrect header format");

            additional_data_size += sizeof(size_t) + 1;

            occ_map[ch] = occurrence;
        }

        tree huff_tree((occurrence_table(occ_map)));
        auto codes = huff_tree.get_code_char();

        std::vector<char> encoded;

        std::uint8_t byte;
        std::string curr_code;
        std::size_t max_code_length = 0;

        for (const auto &tmp: codes)
            max_code_length = std::max(max_code_length, tmp.first.length());

        in.read(reinterpret_cast<char *>(&byte), 1);
        std::size_t read_data = 0;
        while (!in.eof())
        {
            char ch_tmp;
            in.read(reinterpret_cast<char *>(&ch_tmp), 1);
            read_data += 1;

            for (int bit_pos = 0; bit_pos < constants::bits_in_byte; bit_pos++)
            {
                if (in.eof() && bit_pos >= constants::bits_in_byte - extra_bits)
                    break;

                curr_code += ((byte >> bit_pos) & 1) ? '1' : '0';
                auto ch = codes.find(curr_code);

                if (ch != codes.end())
                {
                    encoded.push_back(ch->second);
                    curr_code = "";
                }

                if (curr_code.length() > max_code_length)
                    throw std::invalid_argument("Unexpected code was found");
            }

            byte = ch_tmp;
        }

        std::ofstream out(out_file, std::ios::out);
        for (auto data_byte: encoded)
            out.write(&data_byte, 1);

        std::cout << read_data << std::endl;
        std::cout << encoded.size() << std::endl;
        std::cout << additional_data_size << std::endl;

        out.close();
    }
}