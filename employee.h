#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee
{
    public:
    int employeeId;
    int companyId;
    int employeeGrade;
    int employeeSalary;

    Employee(int EmployeeId, int CompanyId ,int EmployeeGrade);
    ~Employee() = default;
    Employee(const Employee& employee) = default;
    Employee& operator=(const Employee& other) = default;

};

#endif /*EMPLOYEE_H*/