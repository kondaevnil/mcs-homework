#include "employees.h"

#include <utility>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include "bin_manip.h"

namespace Emlpoy
{
    Employee::Employee(std::string name, int32_t base_salary) : name_(std::move(name)), base_salary_(base_salary) {}

    Developer::Developer() : Employee("", 0), has_bonus_(false) {}

    Developer::Developer(std::string name, int32_t base_salary, bool has_bonus) : Employee(std::move(name), base_salary),
        has_bonus_(has_bonus) {}

    [[nodiscard]] int Developer::salary() const
    {
        int salary = base_salary_;

        if (has_bonus_)
            salary += 1000;

        return salary;
    }

    void Developer::print_bin(std::ofstream &os) const
    {
        os << manip::write_le_int32(1) << manip::write_str(name_)
            << manip::write_le_int32(base_salary_) << manip::write_bool(has_bonus_);
    }

    void Developer::print_text(std::ostream &os) const
    {
        os << "Developer\nName: " << name_ << "\nBase Salary: " << base_salary_ << "\nHas bonus: " << (has_bonus_ ? "+\n" : "-\n");
    }

    void Developer::get_bin(std::ifstream &is)
    {
        is >> manip::read_str(name_) >> manip::read_le_int32(base_salary_) >> manip::read_bool(has_bonus_);
    }

    void Developer::get_text(std::istream &is)
    {
        bool has_bonus;

        is >> has_bonus;
        if (is.fail())
            throw std::invalid_argument("No bonus was specified.");

        has_bonus_ = has_bonus;
    }

    SalesManager::SalesManager() : Employee("", 0), sold_nm_(0), price_(0) {}

    SalesManager::SalesManager(std::string name, int32_t base_salary, int32_t sold_nm, int32_t price) :
        Employee(std::move(name), base_salary), sold_nm_(sold_nm), price_(price) {}

    [[nodiscard]] int SalesManager::salary() const
    {
        return int(base_salary_ + sold_nm_ * price_ * 0.01);
    }

    void SalesManager::print_bin(std::ofstream &os) const
    {
        os << manip::write_le_int32(2) << manip::write_str(name_) << manip::write_le_int32(base_salary_)
            << manip::write_le_int32(sold_nm_) << manip::write_le_int32(price_);
    }

    void SalesManager::print_text(std::ostream &os) const
    {
        os << "Sales Manager\nName: " << name_ << "\nBase Salary: " << base_salary_ << "\nSold items: " << sold_nm_
            << "\nItem price: " << price_ << '\n';
    }

    void SalesManager::get_bin(std::ifstream &is)
    {
        is >> manip::read_str(name_) >> manip::read_le_int32(base_salary_)
            >> manip::read_le_int32(sold_nm_) >> manip::read_le_int32(price_);
    }

    void SalesManager::get_text(std::istream &is)
    {
        int32_t sold_nm;
        int32_t price;

        is >> sold_nm;
        if (is.fail())
            throw std::invalid_argument("No sold was specified.");

        is >> price;
        if (is.fail())
            throw std::invalid_argument("No price was specified.");

        sold_nm_ = sold_nm;
        price_ = price;
    }


    std::ostream & operator<<(std::ostream &os, const Employee &employee)
    {
        employee.print_text(os);

        return os;
    }

    std::ofstream & operator<<(std::ofstream &os, const Employee &employee)
    {
        employee.print_bin(os);

        return os;
    }

    std::istream & operator>>(std::istream &is, Employee &employee)
    {
        std::string name;
        int32_t base_salary;

        is >> name;
        if (is.fail())
            throw std::invalid_argument("No name was specified.");

        is >> base_salary;
        if (is.fail())
            throw std::invalid_argument("No base salary was specified.");

        employee.get_text(is);

        employee.base_salary_ = base_salary;
        employee.name_ = name;

        return is;
    }

    std::ifstream & operator>>(std::ifstream &is, Employee &employee)
    {
        employee.get_bin(is);

        return is;
    }

    EmployeesArray::EmployeesArray()
    {
        employees_ = std::vector<std::shared_ptr<Employee>>();
    }

    void EmployeesArray::add(std::shared_ptr<Employee> e)
    {
        employees_.push_back(std::move(e));
    }

    void EmployeesArray::add(const std::string &s)
    {
        std::istringstream iss(s);
        int type;
        std::shared_ptr<Employee> employee;

        iss >> type;
        if (iss.fail())
            throw std::invalid_argument("No type was specified.");

        if (type == 1)
            employee = std::make_shared<Developer>();
        else if (type == 2)
            employee = std::make_shared<SalesManager>();
        else
            throw std::invalid_argument("This type of employee doesn't exist.");

        iss >> *employee;

        add(std::move(employee));
    }

    void EmployeesArray::save(const std::string& s) const
    {
        std::istringstream iss(s);
        std::string filename;

        iss >> filename;
        if (iss.fail())
            throw std::invalid_argument("No filename was found.");

        std::ofstream file(filename, std::ios::binary);

        file << manip::write_le_int32(employees_.size());

        for (const auto &employee: employees_)
            file << *employee;

        file.close();
    }

    void EmployeesArray::load(const std::string &s)
    {
        std::istringstream iss(s);
        std::string filename;

        iss >> filename;
        if (iss.fail())
            throw std::invalid_argument("No filename was found.");

        std::ifstream file(filename, std::ios::binary);

        if (!file.is_open())
            throw std::invalid_argument("File with this name doesn't exist.");

        int type;
        std::shared_ptr<Employee> employee;
        int32_t count;
        file >> manip::read_le_int32(count);

        for (int32_t i = 0; i < count; i++)
        {
            file >> manip::read_le_int32(type);

            if (type == 1)
                employee = std::make_shared<Developer>();
            else if (type == 2)
                employee = std::make_shared<SalesManager>();
            else
                throw std::invalid_argument("This type of employee doesn't exist.");

            file >> *employee;

            add(std::move(employee));
        }

        file.close();
    }

    int32_t EmployeesArray::total_salary() const
    {
        int32_t salary = 0;

        for (const auto &employee: employees_)
            salary += employee->salary();

        return salary;
    }

    std::ostream & operator<<(std::ostream &os, const EmployeesArray &array)
    {
        for (int i = 0; i < array.employees_.size(); i++)
            os << i + 1 << ". " << *array.employees_[i];

        os << "== Total salary: " << array.total_salary() << "\n\n";

        return os;
    }
}