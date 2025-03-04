
#include "library2.h"
#include "ds_manager.h"


void *Init(int k)
{
    if(k<=0)
    {
        return NULL;
    }
    DsManager* CDS=new DsManager(k);
    return (void*)CDS;
}

StatusType AddEmployee(void *DS, int employeeID, int companyID, int grade)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsAddEmployee(employeeID,companyID,grade);
}

StatusType RemoveEmployee(void *DS, int employeeID)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsRemoveEmployee(employeeID);
}

StatusType AcquireCompany(void *DS, int companyID1, int companyID2, double factor)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsAcquireCompany(companyID1,companyID2,factor);
}

StatusType EmployeeSalaryIncrease(void *DS, int employeeID, int salaryIncrease)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsEmployeeSalaryIncrease(employeeID,salaryIncrease);
}

StatusType PromoteEmployee(void *DS, int employeeID, int bumpGrade)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsPromoteEmployee(employeeID,bumpGrade);
}

StatusType SumOfBumpGradeBetweenTopWorkersByGroup(void *DS, int companyID, int m)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsSumOfBumpGradeBetweenTopWorkersByGroup(companyID,m);
}

StatusType AverageBumpGradeBetweenSalaryByGroup(void *DS, int companyID, int lowerSalary, int higherSalary)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsAverageBumpGradeBetweenSalaryByGroup(companyID,lowerSalary,higherSalary);
}

StatusType CompanyValue(void *DS, int companyID)
{
    if(DS==NULL)
    {
        return INVALID_INPUT;
    }
    return static_cast<DsManager*>(DS)->DsCompanyValue(companyID);
}

/*
 * 20 point Bonus function:
StatusType BumpGradeToEmployees(void *DS, int lowerSalary, int higherSalary, int bumpGrade);
 */

void Quit(void** DS)
{
    delete (DsManager*)*DS;
    *DS=NULL;
}
