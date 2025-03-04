#include "rank_tree.h"

static int getMax(int first, int second)
{
    return (first>second)?first:second;
}

int AvlRankTree::calculateBalanceFactor(Node* node) const
{
    return getNodeHeight(node->left)-getNodeHeight(node->right);
}

AvlRankTree::Node* AvlRankTree::getMinNode(Node* node) const
{   
    if(node==nullptr)
    {
        return nullptr;
    }
    if(node->left==nullptr)
    {
        return node;
    }
    return getMinNode(node->left);
}

AvlRankTree::Node* AvlRankTree::getMaxNode(Node* node) const
{
    if(node==nullptr)
    {
        return nullptr;
    }
    if(node->right==nullptr)
    {
        return node;
    }
    return getMaxNode(node->right);   
}

int AvlRankTree::getNodeSalary(Node* node) const
{
    return (node==nullptr)?-1:node->salary;
}

int AvlRankTree::getNodeGrade(Node* node) const
{
    return (node==nullptr)?-1:node->grade;
}

int AvlRankTree::getNodeSumGradeInSubTree(const Node* node) const
{   
    return (node==nullptr)?0:node->sumOfGradeslsInSubTree;
}

int AvlRankTree::getNodeHeight(const Node* node) const
{
    return (node==nullptr)?0:node->height;
}

int AvlRankTree::getNodeId(const Node* node) const
{
    return(node==nullptr)?-1:node->id;
}

int AvlRankTree::getNodeNumPeopleAtAtSubTree(const Node* node) const
{
    return (node==nullptr)?0:node->numPeopleAtSubTree;
}

AvlRankTree::Node* AvlRankTree::createNode(int salary,int id, int grade)
{
    Node* new_node=new Node;
    new_node->salary=salary;
    new_node->id=id;
    new_node->grade=grade;
    new_node->numPeopleAtSubTree=1;
    new_node->sumOfGradeslsInSubTree=grade;
    new_node->father=nullptr;
    new_node->left=nullptr;
    new_node->right=nullptr;
    new_node->height=0;
    return new_node;
}

AvlRankTree::Node* AvlRankTree::duplicateNode(Node* node)
{
    if(node==nullptr)
    {
        return nullptr;
    }
    Node* left_copy=duplicateNode(node->left);
    Node* right_copy=duplicateNode(node->right);
    Node* new_node=createNode(node->salary,node->id,node->grade);
    new_node->left=left_copy;
    if(new_node->left!=nullptr)
    {
        new_node->left->father=new_node;
    }
    new_node->right=right_copy;
    if(new_node->right!=nullptr)
    {
        new_node->right->father=new_node;
    }
    new_node->height=getMax(getNodeHeight(new_node->right),getNodeHeight(new_node->left))+1;
    new_node->numPeopleAtSubTree=
    getNodeNumPeopleAtAtSubTree(new_node->right)+getNodeNumPeopleAtAtSubTree(new_node->left)+1;
    new_node->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(new_node->right)
        +getNodeSumGradeInSubTree(new_node->left)
        +new_node->grade;
    return new_node;
}

//we call this function knowing the employee is not in the tree
AvlRankTree::Node* AvlRankTree::insertNode(Node* new_node,Node* target,Node* parent)
{
    /*insert like in a binary tree before fixing by rolling*/ 
    if(target==nullptr)
    {
        new_node->father=parent;
        new_node->height=getMax(getNodeHeight(new_node->right),getNodeHeight(new_node->left))+1;
        new_node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(new_node->left)
        +getNodeNumPeopleAtAtSubTree(new_node->right)+1; 
        new_node->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(new_node->left)
        +getNodeSumGradeInSubTree(new_node->right)
        +new_node->grade;
        return new_node;
    }
    if(new_node->salary < target->salary)
    {
        target->left=insertNode(new_node,target->left,target);
    }
    else if(new_node->salary > target->salary)
    {     
        target->right=insertNode(new_node,target->right,target);
    }
    else //new_node->salary == target->salary
    {
        if (new_node->id > target->id)
        {
            target->right=insertNode(new_node,target->right,target);
        }
        else
        {
            target->left=insertNode(new_node,target->left,target);   
        }
    }
    /*balance the tree- fix by rolling,
    will only get here and enter the if-condition if a balance was changed 
    after adding a node to the tree, will correct recursively until the root*/ 
    if(calculateBalanceFactor(target)>1)  
    {
        if(calculateBalanceFactor(target->left)>=0)
        {
            return rollLeftLeft(target);
        }
        else //BF(root->left)=-1
        {
            return rollLeftRight(target);
        }
    }
    else if(calculateBalanceFactor(target)<-1)
    {
        if(calculateBalanceFactor(target->right)==1)
        {
            return rollRightLeft(target);
        }
        else
        {
            return rollRightRight(target);
        }
    }
    /*correct height and other fields*/
    target->height=getMax(getNodeHeight(target->right),getNodeHeight(target->left))+1;
    target->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(target->right)+
        getNodeNumPeopleAtAtSubTree(target->left)+1;
    target->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(target->right)
        +getNodeSumGradeInSubTree(target->left)
        +target->grade;
    return target;
}

AvlRankTree::Node* AvlRankTree::findNode(Node* node,int salary, int id) const
{
    if(node==nullptr)
    {
        return nullptr; //the level is not in the tree
    }
    if(node->salary==salary)
    {
        if(node->id==id)
        {
            return node;
        }
        else if(node->id>id)
        {
            return findNode(node->left,salary,id);
        }
        else if(node->id<id)
        {
            return findNode(node->right,salary,id);
        }   
    }
    if(node->salary > salary)
    {
        return findNode(node->left,salary,id);
    }
    if(node->salary < salary) 
    {
        return findNode(node->right,salary,id);
    }
    return nullptr;
}

AvlRankTree::Node* AvlRankTree::deleteNode(Node* target, Node* to_remove)
{
    if(target==nullptr || to_remove== nullptr)  
    {                                          
        return nullptr;
    }
    //remove the node
    if (to_remove->salary == target->salary) 
    {
        if(to_remove->id==target->id) //target==to remove
        {
            if(target->left==nullptr && target->right==nullptr) //a leaf
            {
                delete target;
                return nullptr;
            }
            else if(target->left==nullptr)
            {
                Node* tempNode=target->right;
                target->salary=tempNode->salary;
                target->id=tempNode->id;
                target->grade=tempNode->grade;   
                target->right=deleteNode(target->right,target);
            }
            else if(target->right==nullptr)
            {
                Node* tempNode=target->left;
                target->salary=tempNode->salary;
                target->id=tempNode->id;
                target->grade=tempNode->grade; 
                target->left = deleteNode(target->left,target);
            }
            else //there is a left son and a right son
            {
                Node* tempNode=getMinNode(target->right);
                target->salary=tempNode->salary;
                target->id=tempNode->id;
                target->grade=tempNode->grade; 
                target->right=deleteNode(target->right,target);
            }
        }
        else if(to_remove->id > target->id)
        {
            target->right=deleteNode(target->right,to_remove);
        }
        else if (to_remove->id < target->id)
        {
            target->left=deleteNode(target->left,to_remove);
        }
        
    }
    else if(to_remove->salary > target->salary)
    {
        target->right=deleteNode(target->right,to_remove);
    }
    else
    {
        target->left=deleteNode(target->left,to_remove);
    }
    //fix
    target->height=getMax(getNodeHeight(target->right),getNodeHeight(target->left))+1;
    target->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(target->right)
        +getNodeNumPeopleAtAtSubTree(target->left)+1;
    target->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(target->right)
        +getNodeSumGradeInSubTree(target->left)+target->grade;
    //balance the tree
    if(calculateBalanceFactor(target)>1) 
    {
        if(calculateBalanceFactor(target->left)>=0)
        {
            return rollLeftLeft(target);
        }
        else
        {
            return rollLeftRight(target);
        }
    }
    else if(calculateBalanceFactor(target)<-1) 
    {
        if(calculateBalanceFactor(target->right)==1)
        {
            return rollRightLeft(target);
        }
        else
        {
            return rollRightRight(target);
        }
    }
    return target;   
    }

AvlRankTree::Node* AvlRankTree::selectNodeOperation(Node* node, int index) const
{
    if(getNodeNumPeopleAtAtSubTree(node->left)==index-1) //the prev to node got the prev index
    {
        return node;
    }
    else if(getNodeNumPeopleAtAtSubTree(node->left) < index-1) // go more left
    {
        return selectNodeOperation(node->right, index- (getNodeNumPeopleAtAtSubTree(node->left)+1));
    }
    else  //go more right to biggers ranks
    {
        return selectNodeOperation(node->left,index);
    }   
}

AvlRankTree::Node* AvlRankTree::rollLeftLeft(Node* node)
{ //rotate
    Node* tempNode = node->left;
    node->left->father=node->father;
    node->father=tempNode;
    if(tempNode->right!=nullptr)
    {
        tempNode->right->father=node;
    }
    node->left =node->father->right;
    node->father->right=node;
    //update height and other fields
    node->height=getMax(getNodeHeight(node->left),getNodeHeight(node->right))+1;
    node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->right)
        +getNodeNumPeopleAtAtSubTree(node->left)+1;
    node->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(node->right)
        +getNodeSumGradeInSubTree(node->left)+node->grade;
    if(node->father != nullptr)
    {
        node->father->height=getMax(getNodeHeight(node->father->right),getNodeHeight(node->father->left))+1;
        node->father->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->father->right)
        +getNodeNumPeopleAtAtSubTree(node->father->left)+1;
        node->father->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(node->father->right)
        +getNodeSumGradeInSubTree(node->father->left)
        +node->father->grade;
    }
    return node->father;
}
AvlRankTree::Node* AvlRankTree::rollRightRight(Node* node)
{
    //rotate
    Node* temp=node->right;
    node->right->father=node->father;
    node->father=temp;
    node->right=node->father->left;
    if(node->right!=nullptr)
    {
        node->right->father = node;
    }
    node->father->left = node;
    //update heights and such
    node->height=getMax(getNodeHeight(node->right),getNodeHeight(node->left))+1;
    node->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->right)
        +getNodeNumPeopleAtAtSubTree(node->left)+1;
    node->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(node->right)
        +getNodeSumGradeInSubTree(node->left)
        +node->grade;
    if(node->father != nullptr)
    {
        node->father->height=getMax(getNodeHeight(node->father->right),getNodeHeight(node->father->left))+1;
        node->father->numPeopleAtSubTree=getNodeNumPeopleAtAtSubTree(node->father->right)
        +getNodeNumPeopleAtAtSubTree(node->father->left)+1;
        node->father->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(node->father->right)
        +getNodeSumGradeInSubTree(node->father->left)
        +node->father->grade;
    }
    return node->father;  
}

//combination of a single left rotation followed by a single right rotation
AvlRankTree::Node* AvlRankTree::rollRightLeft(Node* node)
{
    node->right = rollLeftLeft(node->right);
    return rollRightRight(node);
}

//combination of a single right rotation followed by a single left rotation
AvlRankTree::Node* AvlRankTree::rollLeftRight(Node* node)
{
    node->left = rollRightRight(node->left);
    return rollLeftLeft(node);
}

void AvlRankTree::emptyTreeAux(Node* node)
{
    if(node==nullptr)
    {
        return;
    }
    if(node->right!=nullptr)
    {
        emptyTreeAux(node->right);
    }
    if(node->left!=nullptr)
    {
        emptyTreeAux(node->left);
    }
    delete node;
    return;
}
AvlRankTree::Node* AvlRankTree::convertSortedArrayToTreeAux(Node* sorted_array[],int start,int end,Node* father)
{
    if(start>end)
    {
        return nullptr;
    }
    int middle=(start+end)/2;
    Node* new_element_in_tree=createNode(getNodeSalary(sorted_array[middle]),
        getNodeId(sorted_array[middle]),getNodeGrade(sorted_array[middle]));
    new_element_in_tree->father=father;
    new_element_in_tree->right = convertSortedArrayToTreeAux(sorted_array, middle + 1, end, new_element_in_tree);
    new_element_in_tree->left = convertSortedArrayToTreeAux(sorted_array, start, middle - 1, new_element_in_tree);
    new_element_in_tree->height=getMax(getNodeHeight(new_element_in_tree->right),getNodeHeight(new_element_in_tree->left))+1;
    new_element_in_tree->numPeopleAtSubTree= getNodeNumPeopleAtAtSubTree(new_element_in_tree->right)
        + getNodeNumPeopleAtAtSubTree(new_element_in_tree->left)+1;
    new_element_in_tree->sumOfGradeslsInSubTree=getNodeSumGradeInSubTree(new_element_in_tree->right)
        + getNodeSumGradeInSubTree(new_element_in_tree->left)
        +new_element_in_tree->grade;
    return new_element_in_tree;
}

void AvlRankTree::printTreeAux(Node* root) const //print inorder style
{
    if(root==nullptr)
    {
        return;
    }
    printTreeAux(root->left);
    std::cout<< "salary:"<< root->salary <<" id:"<< root->id<< "grafe:"<< root->grade<<std::endl;
    std::cout<<"num of people in sub tree:"<<root->numPeopleAtSubTree<<std::endl;
    std::cout<<"sum of grades in sub tree:"<<root->sumOfGradeslsInSubTree <<std::endl;
    std::cout << " - - - - - - - - - - - - - - - - - - - - - - - - - - - - -"<<std::endl;
    printTreeAux(root->right);
}

bool AvlRankTree::isTreeEmpty() const
{
    return (size_of_tree==0);
}
void AvlRankTree::empty()
{
    if(size_of_tree > 0)
    {
    emptyTreeAux(root);
    }
    root = nullptr;
    size_of_tree = 0;
    max = nullptr;
    min=nullptr;
}

AvlRankTree::AvlRankTree():size_of_tree(0),root(nullptr),max(nullptr),min(nullptr){}

AvlRankTree::AvlRankTree(const AvlRankTree& other):size_of_tree(other.size_of_tree),root(nullptr),max(nullptr),min(nullptr)
{
    root=duplicateNode(other.root);
    max=getMaxNode(root);
    min=getMinNode(root);
}

AvlRankTree::~AvlRankTree()
{
    empty();  
}

AvlRankTree& AvlRankTree::operator=(const AvlRankTree &other)
{
    empty();
    root = duplicateNode(other.root);
    size_of_tree = other.size_of_tree;
    max=getMaxNode(root);
    min=getMinNode(root);
    return *this;
}

//it's on the user that the tree will be empty 
void AvlRankTree::convertSortedArrayToTree(Node* dataArr[],int arrSize)
{
    root=convertSortedArrayToTreeAux(dataArr,0,arrSize-1,nullptr);
    size_of_tree=arrSize;
    max=getMaxNode(root);
    min=getMinNode(root);
}

//we call this function knowing the data of the employee isnt in the tree!!
void AvlRankTree::insert(int salary,int id,int grade)
{
    Node* new_employee=createNode(salary,id,grade); 
    root=insertNode(new_employee,root,nullptr);
    min = getMinNode(root);
    max = getMaxNode(root);
    size_of_tree++;
}


AvlRankTree::Node* AvlRankTree::find(int salary,int id)
{
    Node* node=findNode(root,salary,id);
    return node; //may be null
}

//we call this function knowing the data of the certain employee is in the tree!!
void AvlRankTree::remove(int salary,int id)
{
    Node* to_delete=find(salary,id);
    if(to_delete== nullptr)
    {
        return;
    }                                
    root=deleteNode(root,to_delete);
    size_of_tree--;
    min= getMinNode(root);
    max= getMaxNode(root);
}

//the user need to make sure the index is valid before calling this function
//invalid: index<0 or index> size_of_tree
AvlRankTree::Node* AvlRankTree::select(int index) const
{
    Node* node=selectNodeOperation(root,index);
    return node;
}

int AvlRankTree::getHeight() const
{
    return (root==nullptr)?0:root->height;
}

int AvlRankTree::getMaxSalary() const
{
    return getNodeSalary(this->max);
}

void AvlRankTree::printTree() const
{
    printTreeAux(root);
}

AvlRankTree::Iterator::Iterator(Node* node, Node* last):node(node),last(last){}

AvlRankTree::Iterator& AvlRankTree::Iterator::operator=(const Iterator& other_iter)
{
    if(*this == other_iter)
    {
        return *this;
    }
    last = other_iter.last;
    node = other_iter.node;
    return *this;
}

AvlRankTree::Iterator::Iterator(const Iterator& other_iter):node(other_iter.node),
                                            last(other_iter.last){}

AvlRankTree::Node* AvlRankTree::Iterator::operator*() //may return null-the user 
{                                                     //need to be careful
    return node;
}

bool AvlRankTree::Iterator::operator==(const Iterator& iter) const
{
    if(node==iter.node)
    {
        return true;
    }
    return false;
}
bool AvlRankTree::Iterator::operator!=(const Iterator& iter) const
{
    return !(*this == iter);
}

AvlRankTree::Iterator& AvlRankTree::Iterator::operator++()
{
    if(node == last)
    {
        node = nullptr;
    }
    else
    {
        if(node->right!= nullptr)
        {
            node=node->right;
            while(node->left!= nullptr)
            {
                node=node->left;
            }
        }
        else
        {
            Node* tempNode=node;
            node=node->father;
            while((node->father!= nullptr) && (node->right==tempNode)) //came from right son
            {
                tempNode=node;
                node=node->father;
            }
        }

    }
    return *this;
}

AvlRankTree::Iterator AvlRankTree::begin() const
{
    return Iterator(this->min, this->max);
}
AvlRankTree::Iterator AvlRankTree::end() const
{
    return Iterator(nullptr, nullptr);
}

//will merge 2 tree to one in O(n) - on the user to delete the source's trees
AvlRankTree::AvlRankTree(const AvlRankTree& firstTree, const AvlRankTree& secondTree):
        size_of_tree(0),root(nullptr),max(nullptr),min(nullptr)
{
    Iterator firstIter=firstTree.begin();
    Iterator secondIter=secondTree.begin();
    int size=firstTree.size_of_tree+secondTree.size_of_tree;
    Node** sortedArr = new Node*[size]; //will hold copies!!
    int index=0;
    while(firstIter!=firstTree.end() && secondIter!=secondTree.end()) //merge
    {
        if( ((*firstIter)->salary) < ((*secondIter)->salary) )
        {
            Node* copy=createNode(((*firstIter)->salary),((*firstIter)->id),
                    ((*firstIter)->grade));
            //no need to copy fields that are dependent on the sub tree
            sortedArr[index]=copy;
            ++firstIter;
        }
        else if( ((*firstIter)->salary) > ((*secondIter)->salary) )
        {
            Node* copy=createNode(((*secondIter)->salary),((*secondIter)->id),
                    ((*secondIter)->grade));
            //no need to copy fields that are dependent on the sub tree
            sortedArr[index]=copy;
            ++secondIter;
        }
        else if(( ((*firstIter)->salary) == ((*secondIter)->salary) ))
        {
            if( ((*firstIter)->id) < ((*secondIter)->id) )
            {
                Node* copy=createNode(((*firstIter)->salary),((*firstIter)->id),
                    ((*firstIter)->grade));
                //no need to copy fields that are dependent on the sub tree
                sortedArr[index]=copy;
                ++firstIter;

            }
            else //((*firstIter)->id) > ((*secondIter)->id)
            {
                Node* copy=createNode(((*secondIter)->salary),((*secondIter)->id),
                    ((*secondIter)->grade));
                //no need to copy fields that are dependent on the sub tree
                sortedArr[index]=copy;
                ++secondIter;
            }
        }
        index++; 
    }

    while(firstIter != firstTree.end())
    {
        Node* copy=createNode(((*firstIter)->salary),((*firstIter)->id),
                    ((*firstIter)->grade));
        sortedArr[index]=copy;
        ++firstIter;
        index++;
    }

    while(secondIter != secondTree.end()) 
    {
        Node* copy=createNode(((*secondIter)->salary),((*secondIter)->id),
                    ((*secondIter)->grade));
        sortedArr[index]=copy;
        ++secondIter;
        index++;
    }
    
    convertSortedArrayToTree(sortedArr,size); //make copies for itself
    //delete sortedArr
    for(int i=0;i<size;i++) //destroy the copies we made
    {
        delete (sortedArr[i]);
    }
    delete[] sortedArr;
}

//on the user: "this" is empty (call empty before using)
void AvlRankTree::fillFromTree(const AvlRankTree& src)
{
    Node** sortedSrcArr = new Node*[src.size_of_tree];
    int index=0;
    AvlRankTree::Iterator itr=src.begin();
    while(itr!=src.end())
    {
        Node* copy=createNode(((*itr)->salary),((*itr)->id),((*itr)->grade));
        sortedSrcArr[index]=copy;
        ++itr;
        index++;
    }
    this->convertSortedArrayToTree(sortedSrcArr,src.size_of_tree); //make copies for itself
    for(int i=0;i<src.size_of_tree;i++) //destroy the copies we made
    {
        delete (sortedSrcArr[i]);
    }
    delete[] sortedSrcArr;
    this->size_of_tree=src.size_of_tree;
    max=getMaxNode(this->root);
    min=getMinNode(this->root);
}

//we call this function knowing first and second are in the tree- isn't empty
AvlRankTree::Node* AvlRankTree::getCommonFather(Node* first,Node* second,Node* origin)//O(logn)
{
    if((origin->salary > first->salary) && (origin->salary > second-> salary))
    {
        if(origin->left== nullptr)
        {
            return origin;
        }
        else
        {
            return getCommonFather(first,second,origin->left);
        }

    }
    //if both first and second are bigger than the origin ,
    // the common father is at the right size
    if((origin->salary < first->salary) && (origin->salary < second-> salary))
    {
        if(origin->right== nullptr)
        {
            return origin;
        }
        else
        {
            return getCommonFather(first,second,origin->right);
        }
    }
    //special case- first and second have the same salary (different id!!)
    if((origin->salary == first->salary) && (origin->salary == second-> salary))
    {
        if(origin->id==first->id) //origin is min
        {
           return origin;
        }
        if(origin->id==second->id)
        {
            return origin;
        }
        if((origin->id < first->id) && (origin->id < second-> id))
        {
            if(origin->right== nullptr)
            {
                return origin;
            }
            else
            {
                return getCommonFather(first,second,origin->right);
            }
        }
        else if((origin->id > first->id) && (origin->id > second-> id))
        {
            if(origin->left== nullptr)
            {
                return origin;
            }
            else
            {
                return getCommonFather(first,second,origin->left);
            }
        }
    }
    return origin;
}

//first call- node==root, currentfound==null
AvlRankTree::Node* AvlRankTree::findNodeWithSalaryAndMinId(Node* node,int salary,Node* currentfound) const
{
    if(node==nullptr)
    {
        return currentfound; //may be null
    }
    if(node->salary==salary)
    {
        currentfound=node;
        //look in the left size in search for smaller id
        return findNodeWithSalaryAndMinId(node->left,salary,currentfound);
    }
    else if(node->salary < salary)
    {
        return findNodeWithSalaryAndMinId(node->right,salary,currentfound);
    }
    else if (node->salary > salary)
    {
        return findNodeWithSalaryAndMinId(node->left,salary,currentfound);
    }
    return nullptr;
}

AvlRankTree::Node* AvlRankTree::findNodeWithSalaryAndMaxId(Node* node,int salary,Node* currentfound) const
{
    if(node==nullptr)
    {
        return currentfound; //may be null
    }
    if(node->salary==salary)
    {
        currentfound=node;
        //look in the right size in search for bigger id
        return findNodeWithSalaryAndMaxId(node->right,salary,currentfound);
    }
    else if(node->salary < salary)
    {
        return findNodeWithSalaryAndMaxId(node->right,salary,currentfound);
    }
    else if (node->salary > salary)
    {
        return findNodeWithSalaryAndMaxId(node->left,salary,currentfound);
    }
    return nullptr;
}

bool AvlRankTree::nodesAreEquall(Node* first, Node* second)
{
    return ( (first->salary==second->salary) && (first->id==second->id) );
}

int AvlRankTree::getTotalNumPeopleAtTree() const
{
    return (root== nullptr)?0:root->numPeopleAtSubTree;
}

//before caling this func make sure there are at least m people in the tree!!
long long int AvlRankTree::getSumGradesOfMHighestSalaryEmployees(long long int* sum,int m,Node* node)
{
    if(node->right!= nullptr)
    {
        if(node->right->numPeopleAtSubTree>=m)
        {
            return getSumGradesOfMHighestSalaryEmployees(sum,m,node->right);
        }
        else
        {  //take all the right tree and see how much you can take from node
            *sum+=node->right->sumOfGradeslsInSubTree;
            m=m-node->right->numPeopleAtSubTree;
            if(m==1)  //we counted m-1 people
            {
                *sum+=node->grade;
                return *sum;
            }
            else
            {
                *sum+=node->grade;
                m=m-1;
                return getSumGradesOfMHighestSalaryEmployees(sum,m,node->left); //node->left exist
                                                              //because m>= num people at three
            }
        }
    }
    else
    {
        if(m==1)
        {
            *sum+=node->grade;
            return *sum;
        }
        else
        {
            *sum+=node->grade;
            m=m-1;
            return getSumGradesOfMHighestSalaryEmployees(sum,m,node->left); //node->left surly exist
        }
    }
}

AvlRankTree::Node* AvlRankTree::getRoot() const
{
    return this->root;
}

long long int AvlRankTree::getSumGradesOfEmployeeWithSalaryBetweenBounds(int lowSalary,int highSalary,long long int* num)
{
    if(lowSalary==highSalary)
    {
        Node* tempNode=nullptr;
        Node* minSalaryNode=findNodeWithSalaryAndMinId(root,lowSalary,tempNode);
        if(minSalaryNode==nullptr) //there is no employee with such salary
        {
            *num+=0;
            return 0;
        }
        else
        {
            tempNode=nullptr;
            Node* maxSalaryNode=findNodeWithSalaryAndMaxId(root,highSalary,tempNode);
            if(maxSalaryNode->id == minSalaryNode->id) // there is only one employee in range
            {
                *num+=1;
                return maxSalaryNode->grade;
            }
            else //there are at least 2 employees in range
            {
                Node* commonFather=getCommonFather(minSalaryNode,maxSalaryNode,root);
                long long int numPeople=0;
                long long int sum=countSumGradeInRange(commonFather,minSalaryNode,maxSalaryNode,&numPeople);
                *num+=numPeople;
                return sum;
            }

        }
    }
    else //low salary not equall high salary
    {
        bool lowIsFake=false;
        bool highIsFake=false;
        int numFakeNodes=0;
        Node* temp=nullptr;
        Node* lowSalaryMinIdNode=findNodeWithSalaryAndMinId(root,lowSalary,temp);
        if(lowSalaryMinIdNode==nullptr) //low salary isnt in the tree
        {
            insert(lowSalary,0,0);
            lowIsFake=true;
            numFakeNodes++;
        }
        temp=nullptr;
        Node* highSalaryMaxIdNode=findNodeWithSalaryAndMaxId(root,highSalary,temp);
        if(highSalaryMaxIdNode==nullptr)
        {
            insert(highSalary,0,0);
            highIsFake=true;
            numFakeNodes++;
        }
        //we inserted the nodes, now we can find them
        temp= nullptr;
        lowSalaryMinIdNode= findNodeWithSalaryAndMinId(root,lowSalary,temp);
        temp= nullptr;
        highSalaryMaxIdNode= findNodeWithSalaryAndMaxId(root,highSalary,temp);
       // lowSalaryMinIdNode=find(lowSalary,0);
       // highSalaryMaxIdNode=find(highSalary,0);
        Node* commonFather=getCommonFather(lowSalaryMinIdNode,highSalaryMaxIdNode,root);
        long long int numPeople=0;
        long long int sum=countSumGradeInRange(commonFather,lowSalaryMinIdNode,highSalaryMaxIdNode,&numPeople);
        numPeople-=numFakeNodes;
        if(lowIsFake==true)
        {
            remove(lowSalary,0);
        }
        if(highIsFake==true)
        {
            remove(highSalary,0);
        }
        if(numPeople==0)
        {
            *num+=0;
            return 0;
        }
        *num+=numPeople;
        return sum;
    }
}

//start is common father
long long int AvlRankTree::countSumGradeInRange(Node* start,Node* min,Node* max,long long int* numPeople)
{
    if(start==nullptr)
    {
        *numPeople+=0;
        return 0;
    }
    long long int sum=0;
    if(start->salary==min->salary && start->id==min->id) //common father is min
    {
        *numPeople+=1;
        sum+=start->grade;
        //left son irrelevant , right son is on the road to max
        sum+=sumGradesUntilReachMax(start->right,max,numPeople);
        return sum;
    }
    else if((start->salary==max->salary && start->id==max->id)) //common father is max
    {
        *numPeople+=1;
        sum+=start->grade;
        //right son irrelavent, min is somewhere in the left son sub tree
        sum+=sumGradesUntilReachMin(start->left,min,numPeople);
        return sum;
    }
    else // min somewhere in left son , max somewhere in right son
    {
        *numPeople+=1;
        sum+=start->grade;
        int rightSum=sumGradesUntilReachMax(start->right,max,numPeople);
        int leftSum=sumGradesUntilReachMin(start->left,min,numPeople);
        sum=sum+rightSum+leftSum;
        return sum;
    }
    
}

long long int AvlRankTree::sumGradesUntilReachMax(Node* start,Node* max,long long int* numPeople)
{
    if(start==nullptr)
    {
        return 0;
    }
    if(start->salary < max->salary)
    {
        *numPeople+=getNodeNumPeopleAtAtSubTree(start->left)+1;
        long long int sum=getNodeSumGradeInSubTree(start->left)+start->grade;
        return sum+sumGradesUntilReachMax(start->right,max,numPeople);
    }
    else if(start->salary > max->salary)
    {
        return sumGradesUntilReachMax(start->left,max,numPeople);
    }
    else //start->salary == max->salary
    {
        if(start->id < max->id)
        {
            *numPeople+=getNodeNumPeopleAtAtSubTree(start->left)+1;
            long long int sum=getNodeSumGradeInSubTree(start->left)+start->grade;
            return sum+sumGradesUntilReachMax(start->right,max,numPeople);
        }
        else if(start->id == max->id) //start->id cant be bigger than max
                                    //if equal we arrived at max
        {                        
            *numPeople+=getNodeNumPeopleAtAtSubTree(start->left)+1;
            long long int sum=getNodeSumGradeInSubTree(start->left)+start->grade;
            return sum;
        }
    }
    return 0;
}

long long int AvlRankTree::sumGradesUntilReachMin(Node* start,Node* min, long long int* numPeople)
{
    if(start==nullptr)
    {
        return 0;
    }
    if(start->salary > min->salary)
    {
        *numPeople+=getNodeNumPeopleAtAtSubTree(start->right)+1;
        long long int sum=getNodeSumGradeInSubTree(start->right)+start->grade;
        return sum+sumGradesUntilReachMin(start->left,min,numPeople);
    }
    else if(start->salary < min->salary)
    {
        return sumGradesUntilReachMin(start->right,min,numPeople);
    }
    else //start->salary == min->salary
    {
        if(start->id > min->id)
        {
            *numPeople+=getNodeNumPeopleAtAtSubTree(start->right)+1;
            long long int sum=getNodeSumGradeInSubTree(start->right)+start->grade;
            return sum+sumGradesUntilReachMin(start->left,min,numPeople);
        }
        else if(start->id == min->id) //start->id cant be smaller than min
                                    //if equal we arrived at min
        {                        
            *numPeople+=getNodeNumPeopleAtAtSubTree(start->right)+1;
            long long int sum=getNodeSumGradeInSubTree(start->right)+start->grade;
            return sum;
        }
    }
    return 0;
}




