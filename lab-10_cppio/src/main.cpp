#include "employees.h"
#include <iostream>

int main()
{
    auto employee_array = new Emlpoy::EmployeesArray();
    std::string input;
    bool stop = false;

    while (!stop)
    {
        std::cin >> input;

        if (input == "exit")
            stop = true;
        else if (input == "load")
        {
            getline(std::cin, input);
            employee_array->load(input);
        }
        else if (input == "save")
        {
            getline(std::cin, input);
            employee_array->save(input);
        }
        else if (input == "add")
        {
            getline(std::cin, input);
            employee_array->add(input);
        }
        else if (input == "list")
            std::cout << *employee_array;
    }
}