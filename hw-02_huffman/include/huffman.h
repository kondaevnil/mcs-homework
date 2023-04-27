#ifndef HW2_HUFFMAN_H
#define HW2_HUFFMAN_H

#include <memory>
#include <cstddef>
#include <queue>
#include <map>
#include <vector>
#include <string>
#include "bit_manip.h"
#include <unordered_map>

namespace hw2::huffman
{
    class archiver
    {
    public:
        static void archive(const std::string &in_file, const std::string &out_file);
        static void unarchive(const std::string &in_file, const std::string &out_file);
    };

    struct occurrence_table
    {
        std::map<char, std::size_t> occurrence_;
        explicit occurrence_table(const std::vector<char> &bytes);
        explicit occurrence_table(const std::map<char, std::size_t> &occ_map);
    };

    class tree
    {
        struct node
        {
            using nodeptr = std::shared_ptr<node>;

            nodeptr left;
            nodeptr right;

            char value_{};
            std::size_t occurrence_;

            explicit node(std::pair<char, std::size_t> pr);
            node(nodeptr le, nodeptr ri);
            bool operator>(const node &other) const;
        };

        using nodeptr = typename node::nodeptr;
        nodeptr root = nullptr;
        std::map<char, std::string> char_code;
        //std::map<std::string, char> code_char;
        std::unordered_map<std::string, char> code_char;

        void build_codes(const nodeptr &current, const std::string &code);

    public:
        explicit tree(const occurrence_table &table);
        std::string & get_code(char ch);
        [[nodiscard]] std::uint32_t get_alphabet_size() const;
        //[[nodiscard]] const std::map<std::string, char> & get_code_char() const;
        [[nodiscard]] const std::unordered_map<std::string, char> & get_code_char() const;
    };
}

#endif // HW2_HUFFMAN_H