
#include "dynamic_hash_table.h"
#include "exceptions.h"
#include "iostream"


const int EXPANTION_RATE=2;

DynamicHashTable::DynamicHashTable():arr(new LinkedList<shared_ptr<Employee>>*[INITIAL_SIZE]),
                                                       numEmployees(0),tableSize(INITIAL_SIZE)
{
    for (int i = 0; i < INITIAL_SIZE; i++)
    {
        arr[i] = new LinkedList<shared_ptr<Employee>>();
    }

};

DynamicHashTable::~DynamicHashTable()
{
    for(int i=0;i<tableSize;i++)
    {
        delete arr[i];
    }
    delete[] arr;
}



int DynamicHashTable::activateHashTableFunction(int employeeId) const
{
    return employeeId % tableSize;
}


Node<shared_ptr<Employee>>* DynamicHashTable::getEmployeerById(int employeeId) const
{
    int place_at_arr=activateHashTableFunction(employeeId);
    LinkedList<shared_ptr<Employee>>* curr=(arr[place_at_arr]);
    if(curr!= nullptr)
    {
        Node<shared_ptr<Employee>>* iter=curr->head;
        while (iter != nullptr)
        {
            if(iter->data->employeeId==employeeId)
            {
                return iter;
            }
            iter=iter->next; //advanve in the list at arr[place_at_arr]
        }
    }
    return nullptr;
}

bool DynamicHashTable::find(int employeeId) const
{
    Node<shared_ptr<Employee>>* employee=this->getEmployeerById(employeeId);
    return ( employee != nullptr);
}

void DynamicHashTable::insert(const shared_ptr<Employee>& employee)
{
    int employee_id=employee->employeeId;
    if(this->find(employee_id)==true)
    {
        throw employeeAlreadyExist();
    }
    if(numEmployees == tableSize -1) //need to expand the array
    {
        int old_table_size=tableSize;
        tableSize+=INITIAL_SIZE; //in order to expand by two
        makeDifferentSizeTable(old_table_size, tableSize);
    }
    int index_to_enter_employee=activateHashTableFunction(employee_id);
    arr[index_to_enter_employee]->insert(employee);
    numEmployees=numEmployees+1;
}

void DynamicHashTable::remove(int employeeId) 
{
    Node<shared_ptr<Employee>>* employee=getEmployeerById(employeeId);
    if(employee==nullptr)
    {
        throw employeeDoesntExist();
    }
    int shrink_indicator=(tableSize / (EXPANTION_RATE*EXPANTION_RATE) ) +1 ;
    if(numEmployees==shrink_indicator) // the arr size will be 1/4 of the
    {               //tableSize after removing the employee--> need to shrink
        int old_table_size=tableSize;
        tableSize= (tableSize / EXPANTION_RATE); //smaller by 2
        makeDifferentSizeTable(old_table_size,tableSize);
    }
    employee=getEmployeerById(employeeId);
    int index_of_employee_in_arr=activateHashTableFunction(employeeId);
    arr[index_of_employee_in_arr]->remove(employee);
    numEmployees= numEmployees-1;
}

void DynamicHashTable::makeDifferentSizeTable(int old_table_size, int new_table_size)
{
    LinkedList<shared_ptr<Employee>>** new_table;
    try
    {
        new_table=new LinkedList<shared_ptr<Employee>>*[new_table_size];
        for(int i=0;i<new_table_size;i++)
        {
            new_table[i]= new LinkedList<shared_ptr<Employee>>();
        }
    }
    catch(const std::bad_alloc &error)
    {
        tableSize=old_table_size; //couldn't make a bigger/smaller table
        //so the size of the table is the original size of it
        throw error; //don't change anything and report an error - will exit from here
    }
    for(int i=0;i<old_table_size;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        Node<shared_ptr<Employee>>* list_iter= nullptr;
        if(curr!= nullptr)
        {
            list_iter=curr->head;
        }
        while(list_iter != nullptr)
        {

            shared_ptr<Employee> employee_to_insert=list_iter->data;
            int to_insert_employee_id=employee_to_insert->employeeId;
            try
            {
                //every insertion of an employee could throw bad_alloc
                int index=activateHashTableFunction(to_insert_employee_id);
                new_table[index]->insert(employee_to_insert);
            }
            catch(const std::bad_alloc& error)
            {
                //undo the changes
                tableSize=old_table_size;
                delete[] new_table;
                //report an error
                throw error;
            }
            list_iter=list_iter->next;
        }
    }
    for(int i=0;i<old_table_size;i++)  
    {
        delete arr[i];
    }
    delete[] arr; //delete the old table
    arr=new_table;
}

int DynamicHashTable::getNumEmployeesAtHash() const
{
    return this->numEmployees;
}
int DynamicHashTable::getTableSize() const
{
    return this->tableSize;
}


void DynamicHashTable::setEmployeesCompanyId(int companyId)
{
    for(int i=0;i<this->tableSize;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        if(curr!= nullptr)
        {
            Node<shared_ptr<Employee>>* list_iter= (arr[i])->head;
            while(list_iter!= nullptr)
            {
                shared_ptr<Employee> employee=list_iter->data;
                if(employee!= nullptr)
                {
                   employee->companyId=companyId;
                }
                list_iter=list_iter->next;
            }
        }
    }
}



void DynamicHashTable::print() const  //for debug
{
    for(int i=0;i<tableSize;i++)
    {
        LinkedList<shared_ptr<Employee>>* curr=(arr[i]);
        if(curr!= nullptr)
        {
            Node<shared_ptr<Employee>>* list_iter=curr->head;
            while(list_iter!= nullptr)
            {
                shared_ptr<Employee> employee_to_insert=list_iter->data;
                std::cout<<employee_to_insert->employeeId<<std::endl;
                list_iter=list_iter->next;
            }
        }

    }
}

void DynamicHashTable::mergeHash(DynamicHashTable& src)
{
    for (int i = 0; i < src.tableSize; i++)
    {
        LinkedList<shared_ptr<Employee>>* list_iter=(src.arr[i]);
        if(list_iter!= nullptr)
        {
            Node<shared_ptr<Employee>>* curr=list_iter->head;
            while(curr!= nullptr)
            {
                this->insert(curr->data);
                curr=curr->next;
            }
        }
    }
}


