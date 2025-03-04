#ifndef COMPANY_H
#define COMPANY_H

#include "dynamic_hash_table.h"
#include "rank_tree.h"

class Company
{
    public:
    int companyId;
    int originalValue; //the original value is the same as the companyId
    double factorBonus; // if a company acquires another company its value increase
                        //this field will help us calculate the final value
    int parentCompanyId;
    double parentCompanyExtra;
    AvlRankTree companyEmployeesWithSalaryAboveZeroTree;
    int numEmployeesWithSalaryZero;
    int sumGradesOfEmployeesWithZeroSalary;
    DynamicHashTable companyEmployeesHash;

    Company(int CompanyId);
    ~Company()=default;
    Company(const Company& company);
    void mergeCompanies(Company* company);
    Company& operator=(const Company& company)=delete;
};


#endif /*COMPANY_H*/