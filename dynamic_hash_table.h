#ifndef DYNAMIC_HASH_TABLE_H
#define DYNAMIC_HASH_TABLE_H

#include <stdlib.h>
#include "linked_list.h"
#include "employee.h"
#include <memory>
using std::shared_ptr;
                            
const int INITIAL_SIZE=12;
class DynamicHashTable //chain hashTable
{
    private:
    //helper function- change the size of the table
    // -make it smaller or bigger
    void makeDifferentSizeTable(int old_table_size, int new_table_size);

    public:
    LinkedList<shared_ptr<Employee>>** arr; //every cell in the array is a linked list
                                        //of type shared_ptr of type Employee
    int numEmployees; 
    int tableSize;

    DynamicHashTable();

    ~DynamicHashTable();

    DynamicHashTable(const DynamicHashTable& other):
        arr(new LinkedList<shared_ptr<Employee>>*[other.tableSize]),
        numEmployees(other.numEmployees),tableSize(other.tableSize)
    {
        for (int i = 0; i < other.tableSize; i++)
        {
            arr[i]=other.arr[i]; 
        }
    }

    DynamicHashTable& operator=(const DynamicHashTable& other)=delete;
    int activateHashTableFunction(int employeeId) const;
    Node<shared_ptr<Employee>>* getEmployeerById(int employeeId) const;

    /* hashTable's operations */
    bool find(int employeeId) const;
    void insert(const shared_ptr<Employee>& employee);
    void remove(int employeeId);
    int getNumEmployeesAtHash() const;
    int getTableSize() const;
    void setEmployeesCompanyId(int companyId);

    void print() const;
    void mergeHash(DynamicHashTable& src);

};


#endif /*DYNAMIC_HASH_TABLE_H*/