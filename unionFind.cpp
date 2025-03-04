#include "unionFind.h"

unionFind::unionFind(int companiesNum):numCompanies(companiesNum),
                                       parentArr(new int[companiesNum +1]()),companiesSizeArr(new int[companiesNum + 1]()),
                                       companiesArray(new Company*[companiesNum +1])
{
    for (int i = 1; i < companiesNum+1; ++i) {
        parentArr[i] = -1;
        companiesSizeArr[i] = 1;
        companiesArray[i] = new Company(i);
    }
}

unionFind::~unionFind() {
    delete[] parentArr;
    delete[] companiesSizeArr;
    for(int i=1;i<numCompanies+1;i++)
    {
        delete companiesArray[i];
    }
    delete[] companiesArray;
}

//void unionFind::Makeset(int x) {
//    this->parents[x] = -1;
//    this->size_[x] = 1;
//}

int unionFind::Find(int x) {   //assume x valid
    double sum_bonus=0; //variable to count the sum of the factor bonus of companies
    //along the route from the company to the root, not including the root!
    int parent = this->parentArr[x];
    int old_p = x;
    while (parent > -1) {
        sum_bonus+=companiesArray[old_p]->factorBonus;
        old_p = parent;
        parent = this->parentArr[parent];
    }
    //shrink paths
    double substract=0;
    parent = old_p;
    int temp = x;
    int new_temp;
    while (temp != parent) {
        new_temp = parentArr[temp];
        parentArr[temp] = parent;
        double temp_old_factor=companiesArray[temp]->factorBonus;
        companiesArray[temp]->factorBonus=sum_bonus-substract;
        substract+=temp_old_factor;
        temp = new_temp;
    }
    return parent;
}



void unionFind::Union(int acquire, int target,double factor) {
    int p = this->Find(acquire);
    int q = this->Find(target);
    if (p == q) return;
    if (companiesSizeArr[p] > companiesSizeArr[q]) {
        companiesSizeArr[p] += companiesSizeArr[q];
        companiesSizeArr[q] = 0;
        parentArr[q] = p;
        companiesArray[p]->mergeCompanies(companiesArray[q]);
        //update companies's value: 
        
        double extra=(companiesArray[q]->parentCompanyId+
        companiesArray[q]->parentCompanyExtra)*factor;
        companiesArray[p]->parentCompanyExtra+=extra;
        companiesArray[p]->factorBonus+=extra;
        companiesArray[q]->factorBonus-=companiesArray[p]->factorBonus;
        // if(companiesArray[q]->parentCompany!=companiesArray[q]->companyId)
        // {
        //     companiesArray[p]->factorBonus=companiesArray[p]->factorBonus+
        //             companiesArray[q]->factorBonus*factor+
        //             (companiesArray[companiesArray[q]->parentCompany]->factorBonus+
        //                     companiesArray[companiesArray[q]->parentCompany]->originalValue)*factor;
        // }
        // else
        // {
        //     companiesArray[p]->factorBonus=companiesArray[p]->factorBonus+
        //             (companiesArray[q]->originalValue+companiesArray[q]->factorBonus)*factor;
        // }
        // companiesArray[q]->factorBonus-=companiesArray[p]->factorBonus;
        // companiesArray[q]->parentCompany=companiesArray[p]->parentCompany;

//        double q_val=(companiesArray[q]->factorBonus)+(companiesArray[q]->originalValue);
//        double p_extra=q_val*factor;
//        companiesArray[p]->factorBonus+=p_extra;
//        companiesArray[q]->factorBonus-=(companiesArray[p]->factorBonus);
    }
    else if (companiesSizeArr[p] <= companiesSizeArr[q]) {
        companiesSizeArr[q] += companiesSizeArr[p];
        companiesSizeArr[p] = 0;
        parentArr[p] = q;
        companiesArray[q]->mergeCompanies(companiesArray[p]);
        //update companies's value:
        double extra=(companiesArray[q]->parentCompanyExtra+
        companiesArray[q]->parentCompanyId)*factor;
        companiesArray[p]->factorBonus=companiesArray[p]->factorBonus+
        extra-companiesArray[q]->factorBonus;
        companiesArray[q]->parentCompanyId=companiesArray[p]->parentCompanyId;
        companiesArray[q]->parentCompanyExtra=companiesArray[p]->parentCompanyExtra+extra;
        
        // if(companiesArray[q]->parentCompany!=companiesArray[q]->companyId)
        // {
        //     companiesArray[p]->factorBonus=companiesArray[p]->factorBonus-companiesArray[q]->factorBonus
        //             +(companiesArray[companiesArray[q]->parentCompany]->factorBonus+
        //             companiesArray[companiesArray[q]->parentCompany]->originalValue)*factor
        //             +(companiesArray[q]->factorBonus)*factor;
        // }
        // else
        // {
        //     companiesArray[p]->factorBonus=companiesArray[p]->factorBonus+
        //             (companiesArray[q]->factorBonus+companiesArray[q]->originalValue)*factor
        //             -companiesArray[q]->factorBonus;
        //     companiesArray[q]->parentCompany=companiesArray[p]->parentCompany;
        // }
        // companiesArray[q]->parentCompany=companiesArray[p]->parentCompany;
//        double q_val=(companiesArray[q]->factorBonus)+(companiesArray[q]->originalValue);
//        double p_extra=q_val*factor;
//        double q_bonus=companiesArray[q]->factorBonus;
//        companiesArray[p]->factorBonus=companiesArray[p]->factorBonus+p_extra-q_bonus;
    }
}

Company* unionFind::getCompany(int companyId)
{
    int realCompany=this->Find(companyId);
    return companiesArray[realCompany];
}

double unionFind::calculateCompanyValue(int x)
{
    double value=companiesArray[x]->originalValue;
    int parent = this->parentArr[x];
    int old_p = x;
    while (parent > -1) {
        value+=companiesArray[old_p]->factorBonus;
        old_p = parent;
        parent = this->parentArr[parent];
    }
    value+=companiesArray[old_p]->factorBonus; //count the value of the root
    return (double)value;
}

