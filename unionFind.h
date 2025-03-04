#ifndef UNIONFIND_H
#define UNIONFIND_H

//#include <stdio.h>
//#include <stdlib.h>
#include "company.h"

class unionFind {
    int numCompanies;
    int* parentArr;
    int* companiesSizeArr;
    Company** companiesArray;
public:
    unionFind(int companiesNum); //will make k groups
    ~unionFind();
   
    //void Makeset(int x);//return singletone x
    
    void Union(int acquire, int target,double factor); //return a new union group, p&q deleted.
    
    int Find(int x); //return the group that x belong to

    double calculateCompanyValue(int x);

    //call this function after using find to get the real id of the group
    Company* getCompany(int companyId);
    
};
typedef unionFind* UnionFind;


#endif //UNIONFIND_H