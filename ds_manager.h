#ifndef DS_MANAGER_H
#define DS_MANAGER_H

#include "rank_tree.h"
#include "employee.h"
#include "dynamic_hash_table.h"
#include "exception"
#include "library2.h"
#include "unionFind.h"
#include <memory>
using std::shared_ptr;

class DsManager
{
    public:
    int numOfCompanies;
    DynamicHashTable AllEmployeesHashTable;
    unionFind unionFindCompanies;
    AvlRankTree allEmployeesWithSalaryAboveZeroTree;
    int numEmployeesWithZeroSalary;
    int sumGradesEmployeesWithZeroSalary;

    DsManager(int k):numOfCompanies(k),AllEmployeesHashTable(DynamicHashTable()),
        unionFindCompanies(unionFind(k)),
        allEmployeesWithSalaryAboveZeroTree(AvlRankTree()), numEmployeesWithZeroSalary(0),
        sumGradesEmployeesWithZeroSalary(0){}
    ~DsManager()=default;

    StatusType DsAddEmployee(int employeeID, int companyID, int grade);
    StatusType DsRemoveEmployee(int employeeID);
    StatusType DsAcquireCompany(int companyID1, int companyID2, double factor);
    StatusType DsEmployeeSalaryIncrease(int employeeID, int salaryIncrease);
    StatusType DsPromoteEmployee(int employeeID, int bumpGrade);
    StatusType DsSumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m);
    StatusType DsAverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary);
    StatusType DsCompanyValue(int companyID);
};


#endif /*DS_MANAGER_H*/