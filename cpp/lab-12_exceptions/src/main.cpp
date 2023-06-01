#include <iostream>
#include <string>
#include "matrix.h"

const std::size_t reg_size = 10;

int main()
{
    std::vector<Matrix> registers(reg_size);
    std::string input;

    char r;
    int reg_number1;
    int reg_number2;
    int col;
    int row;
    std::string filename;

    while (std::cin >> input)
    {
        if (input == "load")
        {
            std::cin >> r >> reg_number1 >> filename;
            try
            {
                registers[reg_number1] = std::move(Matrix(filename));
            }
            catch (MatrixException &exception)
            {
                std::cout << exception.what() << "\n";
            }
            catch (std::bad_alloc &)
            {
                std::cout << "Unable to allocate memory.\n";
            }
        }
        else if (input == "print")
        {
            std::cin >> r >> reg_number1;
            registers[reg_number1].print(std::cout);
        }
        else if (input == "add")
        {
            std::cin >> r >> reg_number1 >> r >> reg_number2;
            try
            {
                registers[reg_number1] += registers[reg_number2];
            }
            catch (MatrixException &exception)
            {
                std::cout << exception.what() << "\n";
            }
        }
        else if (input == "mul")
        {
            std::cin >> r >> reg_number1 >> r >> reg_number2;
            try
            {
                registers[reg_number1] *= registers[reg_number2];
            }
            catch (MatrixException &exception)
            {
                std::cout << exception.what() << "\n";
            }
            catch (std::bad_alloc &c)
            {
                std::cout << "Unable to allocate memory.\n";
            }
        }
        else if (input == "elem")
        {
            std::cin >> r >> reg_number1 >> row >> col;
            try
            {
                std::cout << registers[reg_number1].get(row, col) << "\n";
            }
            catch (MatrixException &m)
            {
                std::cout << m.what() << "\n";
            }
        }
        else if (input == "exit")
        {
            return 0;
        }
    }
}
