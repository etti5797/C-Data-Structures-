#include "company.h"

Company::Company(int CompanyId):companyId(CompanyId),originalValue(CompanyId),
    factorBonus(0),parentCompanyId(CompanyId),parentCompanyExtra(0),
    companyEmployeesWithSalaryAboveZeroTree(AvlRankTree()),
    numEmployeesWithSalaryZero(0),sumGradesOfEmployeesWithZeroSalary(0),
    companyEmployeesHash(DynamicHashTable())
{};

Company::Company(const Company& company):companyId(company.companyId),
    originalValue(company.companyId),factorBonus(company.factorBonus),
    companyEmployeesWithSalaryAboveZeroTree(AvlRankTree(company.companyEmployeesWithSalaryAboveZeroTree)),
    numEmployeesWithSalaryZero(company.numEmployeesWithSalaryZero),
    sumGradesOfEmployeesWithZeroSalary(company.sumGradesOfEmployeesWithZeroSalary),
    companyEmployeesHash(company.companyEmployeesHash)
{};

void Company::mergeCompanies(Company* company) // the company is smaller(/equall) than this->company
{
    if(this->companyEmployeesHash.getNumEmployeesAtHash()==0 &&
    company->companyEmployeesHash.getNumEmployeesAtHash()==0)
    {
        return; //nothing to merge (0 employees at both companies)
    }
    this->numEmployeesWithSalaryZero+=company->numEmployeesWithSalaryZero;
    this->sumGradesOfEmployeesWithZeroSalary+=company->sumGradesOfEmployeesWithZeroSalary;
    company->numEmployeesWithSalaryZero=0;
    company->sumGradesOfEmployeesWithZeroSalary=0;

    //merge hash tables
    this->companyEmployeesHash.mergeHash(company->companyEmployeesHash);
    //update the employees's companyId
    this->companyEmployeesHash.setEmployeesCompanyId(this->companyId);

    //merge rank trees
    AvlRankTree mergedTree(company->companyEmployeesWithSalaryAboveZeroTree,
                                this->companyEmployeesWithSalaryAboveZeroTree);
    this->companyEmployeesWithSalaryAboveZeroTree.empty();
    company->companyEmployeesWithSalaryAboveZeroTree.empty();
    this->companyEmployeesWithSalaryAboveZeroTree.fillFromTree(mergedTree);
    mergedTree.empty();
}
