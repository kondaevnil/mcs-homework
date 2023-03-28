#ifndef LAB10_EMPLOYEES_H_INCLUDED
#define LAB10_EMPLOYEES_H_INCLUDED

#include <cstdint>
#include <string>
#include <vector>
#include <ostream>
#include <istream>
#include <memory>

namespace Emlpoy
{
    class Employee
    {
    public:
        Employee() = delete;
        Employee(std::string name, int32_t base_salary);

        virtual void print_text(std::ostream &os) const = 0;

        virtual void print_bin(std::ofstream &os) const = 0;

        virtual void get_text(std::istream &os) = 0;
        friend std::istream& operator>>(std::istream &, Employee &);

        virtual void get_bin(std::ifstream &os) = 0;
        friend std::ifstream& operator>>(std::ifstream &, Employee &);

        [[nodiscard]] virtual int salary() const = 0;

    protected:
        std::string name_;
        int32_t base_salary_{};
    };


    class Developer : public Employee
    {
    public:
        Developer();
        Developer(std::string name, int32_t base_salary, bool has_bonus);

        [[nodiscard]] int salary() const override;

        void print_bin(std::ofstream &os) const override;
        void print_text(std::ostream &os) const override;
        void get_bin(std::ifstream &os) override;
        void get_text(std::istream &os) override;

    private:
        bool has_bonus_;
    };

    class SalesManager : public Employee
    {
    public:
        SalesManager();
        SalesManager(std::string name, int32_t base_salary, int32_t sold_nm, int32_t price);

        [[nodiscard]] int salary() const override;

        void print_bin(std::ofstream &os) const override;
        void print_text(std::ostream &os) const override;
        void get_bin(std::ifstream &os) override;
        void get_text(std::istream &os) override;

    private:
        int32_t sold_nm_, price_;
    };

    class EmployeesArray
    {
    public:
        EmployeesArray();

        void add(std::shared_ptr<Employee> e);
        void add(const std::string &s);

        void save(const std::string& s) const;
        void load(const std::string& s);

        [[nodiscard]] int32_t total_salary() const;

        friend std::ostream & operator<<(std::ostream &, const EmployeesArray &);

    private:
        std::vector<std::shared_ptr<Employee>> employees_;
    };
}

#endif
