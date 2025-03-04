
#include "ds_manager.h"


StatusType DsManager::DsAddEmployee(int employeeID, int companyID, int grade)
{
    if (companyID<=0 || companyID>numOfCompanies || employeeID<=0 || grade<0)
    {
        return INVALID_INPUT;
    }
    if (AllEmployeesHashTable.find(employeeID) == true)
    {
        return FAILURE;
    }
    try
    {
        Company* company=this->unionFindCompanies.getCompany(companyID);
        int employeeTrueCompanyId=company->companyId;
        shared_ptr<Employee> newEmployee(new Employee(employeeID,employeeTrueCompanyId,grade));
        AllEmployeesHashTable.insert(newEmployee); 
        company->companyEmployeesHash.insert(newEmployee);
        this->numEmployeesWithZeroSalary++;
        this->sumGradesEmployeesWithZeroSalary+=grade;
        company->numEmployeesWithSalaryZero++;
        company->sumGradesOfEmployeesWithZeroSalary+=grade;   
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType DsManager::DsRemoveEmployee(int employeeID)
{
    if(employeeID<=0)
    {
        return INVALID_INPUT;
    }
    if(AllEmployeesHashTable.find(employeeID)==false)
    {
        return FAILURE;
    }
    try
    {
        Node<shared_ptr<Employee>>* employeeInfo=AllEmployeesHashTable.getEmployeerById(employeeID);
        int employeeSalary=employeeInfo->data->employeeSalary;
        int employeeGrade=employeeInfo->data->employeeGrade;
        int employeeCompanyId=employeeInfo->data->companyId;
        Company* company=this->unionFindCompanies.getCompany(employeeCompanyId);
        //the employee hold the id of his true company so getCompany work in O(1)
        // (the loop in the Find operator in unionFind dont happen)
        if(employeeSalary==0)
        {
            company->numEmployeesWithSalaryZero--;
            company->sumGradesOfEmployeesWithZeroSalary-=employeeGrade;
            this->numEmployeesWithZeroSalary--;
            this->sumGradesEmployeesWithZeroSalary-=employeeGrade;
        }
        else
        {
            company->companyEmployeesWithSalaryAboveZeroTree.remove(employeeSalary,employeeID);
            this->allEmployeesWithSalaryAboveZeroTree.remove(employeeSalary,employeeID);
        }
        company->companyEmployeesHash.remove(employeeID);
        this->AllEmployeesHashTable.remove(employeeID);
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
    
}

StatusType DsManager::DsAcquireCompany(int companyID1, int companyID2, double factor)
{
    if(companyID1<=0 || companyID1>numOfCompanies || companyID2<=0
    || companyID2>numOfCompanies || factor<=0.0 || companyID1==companyID2)
    {
        return INVALID_INPUT;
    }
    if(unionFindCompanies.getCompany(companyID1)->companyId==
        unionFindCompanies.getCompany(companyID2)->companyId)
    {
        return INVALID_INPUT;
    }
    try
    {
        unionFindCompanies.Union(companyID1,companyID2,factor);
    }
    catch (std::bad_alloc &e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType DsManager::DsCompanyValue(int companyID)
{
    if(companyID<=0 || companyID>numOfCompanies)
    {
        return INVALID_INPUT;
    }
    try
    {
        double value=this->unionFindCompanies.calculateCompanyValue(companyID);
        printf("CompanyValue: %.1f\n", value);

    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;

}

StatusType DsManager::DsEmployeeSalaryIncrease(int employeeID, int salaryIncrease)
{
    if(employeeID<=0 || salaryIncrease<=0)
    {
        return INVALID_INPUT;
    }
    if(AllEmployeesHashTable.find(employeeID)==false)
    {
        return FAILURE;
    }
    try
    {
        Node<shared_ptr<Employee>>* employeeInfo=AllEmployeesHashTable.getEmployeerById(employeeID);
        int employeeGrade=employeeInfo->data->employeeGrade;
        int employeeCompanyId=employeeInfo->data->companyId;
        int employeeOldSalary=employeeInfo->data->employeeSalary;
        int employeeNewSalary=employeeOldSalary+salaryIncrease;
        employeeInfo->data->employeeSalary=employeeNewSalary;
        Company* company=unionFindCompanies.getCompany(employeeCompanyId);
        //the companyId in the employee's data is always updated- find operarion
        //in the union-find will find the company in O(1)
        if(employeeOldSalary==0)
        {
            this->numEmployeesWithZeroSalary--;
            this->sumGradesEmployeesWithZeroSalary-=employeeGrade;
            company->numEmployeesWithSalaryZero--;
            company->sumGradesOfEmployeesWithZeroSalary-=employeeGrade;
            this->allEmployeesWithSalaryAboveZeroTree.insert(employeeNewSalary,employeeID,employeeGrade);
            company->companyEmployeesWithSalaryAboveZeroTree.insert(employeeNewSalary,employeeID,employeeGrade);
        }
        else
        {
            this->allEmployeesWithSalaryAboveZeroTree.remove(employeeOldSalary,employeeID);
            this->allEmployeesWithSalaryAboveZeroTree.insert(employeeNewSalary,employeeID,employeeGrade);
            company->companyEmployeesWithSalaryAboveZeroTree.remove(employeeOldSalary,employeeID);
            company->companyEmployeesWithSalaryAboveZeroTree.insert(employeeNewSalary,employeeID,employeeGrade);
        }

    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
    
}

StatusType DsManager::DsPromoteEmployee(int employeeID, int bumpGrade)
{
    if(employeeID<=0)
    {
        return INVALID_INPUT;
    }
    if(AllEmployeesHashTable.find(employeeID)==false)
    {
        return FAILURE;
    }
    try
    {
        if(bumpGrade<=0) //if bumpGrade<=0 there is no promotion
        {
            return SUCCESS;
        }
        Node<shared_ptr<Employee>>* employeeInfo=AllEmployeesHashTable.getEmployeerById(employeeID);
        int employeeOldGrade=employeeInfo->data->employeeGrade;
        int employeeNewGrade=employeeOldGrade+bumpGrade;
        employeeInfo->data->employeeGrade=employeeNewGrade;
        int employeeSalary=employeeInfo->data->employeeSalary;
        int employeeCompanyId=employeeInfo->data->companyId;
        Company* company=unionFindCompanies.getCompany(employeeCompanyId);
        // getcompany will find the company in O(1) because the employee
        //hold the companyid of his most recent company
        if(employeeSalary==0)
        {
            this->sumGradesEmployeesWithZeroSalary+=bumpGrade;
            company->sumGradesOfEmployeesWithZeroSalary+=bumpGrade;
        }
        else
        {
            this->allEmployeesWithSalaryAboveZeroTree.remove(employeeSalary,employeeID);
            this->allEmployeesWithSalaryAboveZeroTree.insert(employeeSalary,employeeID,employeeNewGrade);
            company->companyEmployeesWithSalaryAboveZeroTree.remove(employeeSalary,employeeID);
            company->companyEmployeesWithSalaryAboveZeroTree.insert(employeeSalary,employeeID,employeeNewGrade);
        }

    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;
}


StatusType DsManager::DsSumOfBumpGradeBetweenTopWorkersByGroup(int companyID, int m)
{
    if(companyID>numOfCompanies || companyID<0 || m<=0)
    {
        return INVALID_INPUT;
    }
    try
    {
        long long int sum=0;
        if(companyID==0)
        {
            if(this->allEmployeesWithSalaryAboveZeroTree.getTotalNumPeopleAtTree()<m)
            {
                return FAILURE;
            }
            long long int sumGrades=this->allEmployeesWithSalaryAboveZeroTree.getSumGradesOfMHighestSalaryEmployees(&sum,m,this->allEmployeesWithSalaryAboveZeroTree.getRoot());
            printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", sumGrades);
        }
        else
        {
            Company* company=unionFindCompanies.getCompany(companyID);
            if(company->companyEmployeesWithSalaryAboveZeroTree.getTotalNumPeopleAtTree()<m)
            {
                return FAILURE;
            }
            long long int sumGrades=company->companyEmployeesWithSalaryAboveZeroTree.getSumGradesOfMHighestSalaryEmployees(&sum,m,company->companyEmployeesWithSalaryAboveZeroTree.getRoot());
            printf("SumOfBumpGradeBetweenTopWorkersByGroup: %lld\n", sumGrades);
        }
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;    
}


StatusType DsManager::DsAverageBumpGradeBetweenSalaryByGroup(int companyID, int lowerSalary, int higherSalary)
{
    if( higherSalary<0 || lowerSalary<0 || lowerSalary>higherSalary 
                            || companyID> numOfCompanies || companyID<0)
    {
        return INVALID_INPUT;
    }
    try
    {
        long long int sum=0;
        long long int num=0;
        if(companyID==0)
        {
            if(lowerSalary==0) //need to count people with salary0
            {
                sum+=this->sumGradesEmployeesWithZeroSalary;
                num+=this->numEmployeesWithZeroSalary;
            }
            //next, count people from the tree in the range, and update "num"
            long long int temp=0;
            sum+=this->allEmployeesWithSalaryAboveZeroTree.getSumGradesOfEmployeeWithSalaryBetweenBounds(lowerSalary,higherSalary,&temp);
            num+=temp;
            if(num==0)// none people in range
            {
                return FAILURE;
            }
            double avg=(((double)sum)/((double)num));
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);
        }
        else
        {
            Company* company=unionFindCompanies.getCompany(companyID);
            if(lowerSalary==0) //need to count people with salary0
            {
                sum+=company->sumGradesOfEmployeesWithZeroSalary;
                num+=company->numEmployeesWithSalaryZero;
            }
            long long int temp=0;
            sum+=company->companyEmployeesWithSalaryAboveZeroTree.getSumGradesOfEmployeeWithSalaryBetweenBounds(lowerSalary,higherSalary,&temp);
            num+=temp;
            if(num==0)
            {

                return FAILURE;
            }
            double avg=(((double)sum)/((double)num));
            printf("AverageBumpGradeBetweenSalaryByGroup: %.1f\n", avg);

        }
    }
    catch (const std::bad_alloc& e)
    {
        return ALLOCATION_ERROR;
    }
    return SUCCESS;   
    
}






