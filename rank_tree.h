#ifndef RANK_TREE_H
#define RANK_TREE_H

#include <iostream>
#include <memory>
#include <assert.h>

class AvlRankTree
{         
    private:
    struct Node{
        friend class AvlRankTree;
        int salary; //first key
        int id; //second key
        int grade; //value
        struct Node*  father;
        struct Node*  left;
        struct Node*  right;
        int height;
        int numPeopleAtSubTree;
        int sumOfGradeslsInSubTree;
    };
    int size_of_tree; //amount of nodes in sub tree
    Node* root;
    Node* max;
    Node* min; 

    /*operation for Node*/
    int calculateBalanceFactor(Node* node) const;
    Node* getMinNode(Node* node) const;
    Node* getMaxNode(Node* node) const;
    int getNodeSalary(Node* node) const;
    int getNodeGrade(Node* node) const;
    int getNodeSumGradeInSubTree(const Node* node) const;
    int getNodeId(const Node* node) const;
    int getNodeHeight(const Node* node) const;
    int getNodeNumPeopleAtAtSubTree(const Node* node) const;
    Node* createNode(int salary,int id, int grade);
    Node* duplicateNode(Node* node);
    Node* insertNode(Node* new_node,Node* target,Node* parent);
    Node* findNode(Node* node,int salary, int id) const;
    Node* deleteNode(Node* target, Node* to_remove);
    Node* selectNodeOperation(Node* node, int index) const;
    void printTreeAux(Node* root) const;
    bool nodesAreEquall(Node* first, Node* second);
    long long int sumGradesUntilReachMax(Node* start,Node* max, long long int* numPeople);
    long long int sumGradesUntilReachMin(Node* start,Node* min,long long int* numPeople);

    /*roll function*/
    Node* rollLeftLeft(Node* node);
    Node* rollRightRight(Node* node);
    Node* rollRightLeft(Node* node);
    Node* rollLeftRight(Node* node);

    // helper func for Constructor && Destructor of AVL
    void emptyTreeAux(Node* node);
    Node* convertSortedArrayToTreeAux(Node* sorted_array[],int start,int end,Node* father);

    //avl rank tree's constructor, destructor, interface functions
    public:
    bool isTreeEmpty() const;
    void empty();
    AvlRankTree();
    AvlRankTree(const AvlRankTree& other);
    ~AvlRankTree();
    AvlRankTree(const AvlRankTree& firstTree, const AvlRankTree& secondTree); //merge
    AvlRankTree& operator=(const AvlRankTree &other);
    void convertSortedArrayToTree(Node* dataArr[],int arrSize);
    void insert(int salary,int id,int grade);
    Node* find(int salary,int id);
    void remove(int salary,int id);
    Node* select(int index) const;
    int getHeight() const;
    int getMaxSalary() const;
    void printTree() const; //for debug
    void fillFromTree(const AvlRankTree& src); //O(n) //fill a tree from data of other tree
    Node* getCommonFather(Node* first,Node* second,Node* origin);
    Node* findNodeWithSalaryAndMinId(Node* node,int salary,Node* currentfound) const;
    Node* findNodeWithSalaryAndMaxId(Node* node,int salary,Node* currentfound) const;
    long long int getSumGradesOfEmployeeWithSalaryBetweenBounds(int lowSalary,int highSalary,long long int* num);
    long long int countSumGradeInRange(Node* start,Node* min,Node* max,long long int* numPeople);
    int getTotalNumPeopleAtTree() const;
    long long int getSumGradesOfMHighestSalaryEmployees(long long int* sum,int m,Node* node);
    Node* getRoot() const;

    //iterator-in order to sort the elements into an array
    class Iterator
    {
        private:
        friend class AvlRankTree;
        Node* node;
        Node* last;
        public:
        Iterator(Node* node, Node* last);
        ~Iterator()=default;
        Iterator& operator=(const Iterator& other_iter);
        Iterator(const Iterator& other_iter);
        Node* operator*();
        bool operator==(const Iterator& iter) const;
        bool operator!=(const Iterator& iter) const;
        Iterator& operator++();
    };
    Iterator begin() const;
    Iterator end() const;

};


#endif