#ifndef LINKED_LIST_H
#define LINKED_LIST_H


template<class T>
class Node
{
public:
    T data;
    Node<T>* next;

    Node<T>(const T& data) :data(data), next(nullptr){};
    Node<T>(const Node<T>& other) :data(other.data),next(other.next){};
    Node& operator=(const Node& other)
    {
        if (this == &other)
        {
            return *this;
        }
        data=other.data;
        next = other.next;
        return *this;
    }
};

template<class T>
class LinkedList
{
public:
    Node<T>* head;
    int size;
    LinkedList<T>() : head(nullptr), size(0) {};
    ~LinkedList()
    {
        Node<T>* curr=head;
        while(curr!=nullptr)
        {
            Node<T>* to_delete=curr;
            curr=curr->next;
            delete to_delete;
        }
    }

    void insert(const T& data)
    {
        if (size == 0)
        {
            head = new Node<T>(data);
            size++;
        }
        else
        {
            Node<T>* new_node = new Node<T>(data);
            new_node->next = head;
            head = new_node;
            size=size+1;
        }
    };

    bool isEmpty() const
    {
        return size==0;
    }


    void remove(Node<T>* to_erase)
    {
        if(to_erase==nullptr || size<=0)
        {
            return;
        }
        if(to_erase==head)
        {
            Node<T>* to_remove=head;
            head=head->next;
            delete to_remove;
            size=size-1;
            return;
        }
        else
        {
            Node<T>* search=head;
            while(search->next !=nullptr)
            {
                if(search->next == to_erase)
                {
                    search->next=to_erase->next;
                    break;
                }
                search=search->next;
            }
            delete to_erase;
            size=size-1;
            return;
        }
    }



};


#endif /*LINKED_LIST_H*/