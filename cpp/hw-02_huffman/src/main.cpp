#include "huffman.h"
#include <string>

//tmp
#include <fstream>

int main(int argc, char *argv[])
{
    if (argc != 6)
        return -1;

    std::string action;
    std::string in_file;
    std::string out_file;

    for (int i = 1; i < 6; i++)
    {
        std::string s = std::string(argv[i]);

        if (s == "-u" || s == "-c")
            action = s;
        else if (s == "-f" && i < 5)
            in_file = std::string(argv[i++ + 1]);
        else if (s == "-o" && i < 5)
            out_file = std::string(argv[i++ + 1]);
        else
            return -1;
    }

    if (action == "-c")
    {
        try
        {
            hw2::huffman::archiver::archive(in_file, out_file);
        }
        catch (std::invalid_argument &exception)
        {
            // TODO
        }
    }
    else
    {
        try
        {
            hw2::huffman::archiver::unarchive(in_file, out_file);
        }
        catch (std::invalid_argument &exception)
        {
            // TODO
        }
    }
}
