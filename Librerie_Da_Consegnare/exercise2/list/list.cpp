//#include <cmath>
#include "list.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------Node: Specific Constructors/Destructors-------------------------- */

// Constructor from Data by Copy
template <typename Data>
inline List<Data>::Node::Node(const Data &val) : elem(val) {}

// Constructor from Data by Move
template <typename Data>
inline List<Data>::Node::Node(Data &&val) noexcept : elem(std::move(val)) {}

// Copy constructor
template <typename Data>
inline List<Data>::Node::Node(const Node &to_copy) : elem(to_copy.elem) {}

// Move constructor
template <typename Data>
inline List<Data>::Node::Node(Node &&to_move) noexcept
{
    std::swap(elem, to_move.elem);
    std::swap(next, to_move.next);
}

// Destructor
template <typename Data>
List<Data>::Node::~Node()
{
    delete next;
}

/* ---------------------------Node: Comparison Operators-------------------------- */

template <typename Data>
bool List<Data>::Node::operator==(const Node &to_cmp) const noexcept
{
    return (elem == to_cmp.elem) && ((next == nullptr && to_cmp.next == nullptr) || ((next != nullptr && to_cmp.next != nullptr) && (*next == *to_cmp.next)));
}

template <typename Data>
inline bool List<Data>::Node::operator!=(const Node &to_cmp) const noexcept
{
    return !( (*this) == to_cmp );
}

/* ---------------------------List: Comparison Operators (float, double)-------------------------- */

/*
template <>
inline bool List<float>::Node::operator==(const Node &to_cmp) const noexcept
{
    constexpr float epsilon = 1e-5F;

    return ( fabs(elem - to_cmp.elem) <= epsilon) && ((next == nullptr && to_cmp.next == nullptr) \
    || ((next != nullptr && to_cmp.next != nullptr) && (*next == *to_cmp.next)));
}

template <>
inline bool List<double>::Node::operator==(const Node &to_cmp) const noexcept
{
    constexpr double epsilon = 1e-9;

    return ( fabs(elem - to_cmp.elem) <= epsilon) && ((next == nullptr && to_cmp.next == nullptr) \
    || ((next != nullptr && to_cmp.next != nullptr) && (*next == *to_cmp.next)));
}
*/

/* ---------------------------List: Specific Constructors/Destructors-------------------------- */

// A list obtained from a TraversableContainer
template <typename Data>
List<Data>::List(const TraversableContainer<Data> &cont)
{
    cont.Traverse
    (
        [this] (const Data &curr)
        {
            InsertAtBack(curr);   
        }
    );    
}

// A list obtained from a MappableContainer
template <typename Data>
List<Data>::List(MappableContainer<Data> &&cont)
{
    cont.Map
    (
        [this] (Data &curr)
        {
            InsertAtBack(std::move(curr));   
        }
    );
}

// Copy constructor
template <typename Data>
List<Data>::List(const List<Data> &lst)
{
    if(lst.head != nullptr)
    {
        head = tail = new Node(lst.head->elem);

        Node * walk_ptr = lst.head->next;
        
        while(walk_ptr)
        {
            InsertAtBack(walk_ptr->elem);
            walk_ptr = walk_ptr->next;
        }
        size = lst.size;
    }
}

// Move constructor
template <typename Data>
List<Data>::List(List<Data> &&lst) noexcept
{
    std::swap(head, lst.head);
    std::swap(tail, lst.tail);
    std::swap(size, lst.size);
}

// Destructor
template <typename Data>
List<Data>::~List()
{
    delete head;
}

/* ---------------------------List: Assignments-------------------------- */

// Copy assignment
template <typename Data>
List<Data> & List<Data>::operator=(const List<Data> &lst)
{
    if(this == &lst)
        return *this;

    if(lst.head != nullptr)
    {
        if(size == 0)
        {
            List<Data> * tmp = new List<Data>(lst);
            std::swap(*this, *tmp);
            delete tmp;
        }
        else 
        {
            Node * lst_walkptr = lst.head;
            Node * this_walkptr = head;

            if(size <= lst.size)
            {
                while(this_walkptr)
                {
                    this_walkptr->elem = lst_walkptr->elem;

                    lst_walkptr = lst_walkptr->next;
                    this_walkptr = this_walkptr->next;
                }

                while(lst_walkptr)
                {
                    InsertAtBack(lst_walkptr->elem);
                    lst_walkptr = lst_walkptr->next;
                }
            }
            else
            {
                while(lst_walkptr)
                {
                    this_walkptr->elem = lst_walkptr->elem;

                    if(lst_walkptr == lst.tail)
                    {
                        tail = this_walkptr;
                        delete tail->next;
                        tail->next = nullptr;
                    }

                    lst_walkptr = lst_walkptr->next;
                    this_walkptr = this_walkptr->next;
                }   
            }      
        }
        size = lst.size;  
    }
    else
        Clear();

    return *this;
}

// Move assignment
template <typename Data>
List<Data> & List<Data>::operator=(List<Data> &&lst) noexcept
{
    std::swap(head, lst.head);
    std::swap(tail, lst.tail);
    std::swap(size, lst.size);

    return *this;
}

/* ---------------------------List: Comparison Operators-------------------------- */

template <typename Data>
inline bool List<Data>::operator==(const List<Data> &lst) const noexcept
{
    return (size == lst.size) && ((head == nullptr && lst.head == nullptr) || ((head != nullptr && lst.head != nullptr) && (*head == *lst.head)));
}

template <typename Data>
inline bool List<Data>::operator!=(const List<Data> &lst) const noexcept
{
    return !((*this) == lst); 
}

/* ---------------------------List: Member Functions (Dictionary) -------------------------- */ 

template <typename Data>
void List<Data>::InsertAtFront(const Data &val)
{
    Node * tmp = new Node(val);

    tmp->next = head;
    head = tmp;

    if(!tail)
        tail = head;
   
    ++size;
}

template <typename Data>
void List<Data>::InsertAtFront(Data &&val)
{
    Node * tmp = new Node(std::move(val));

    tmp->next = head;
    head = tmp;
    
    if(!tail)
        tail = head;
   
    ++size;
}

template <typename Data>
void List<Data>::RemoveFromFront() // (must throw std::length_error when empty)
{
    if(!head)
        throw std::length_error("Empty Exception from List\n");

    Node * tmp = head;
    
    if(head == tail)
        head = tail = nullptr;
    else
        head = head->next;

    tmp->next = nullptr; 
    delete tmp;
    --size; 
}

template <typename Data>
Data List<Data>::FrontNRemove() // (must throw std::length_error when empty)
{
    if(!head)
        throw std::length_error("Empty Exception from List\n");

    Data val(std::move(head->elem)); 
    RemoveFromFront();

    return val;
}

template <typename Data>
void List<Data>::InsertAtBack(const Data &val) // Copy of the value
{
    Node * tmp = new Node(val);

    if(!head)
        head = tmp;
    else
        tail->next = tmp;

    tail = tmp;
    ++size;
}

template <typename Data>
void List<Data>::InsertAtBack(Data &&val) // Move of the value
{
    Node * tmp = new Node(std::move(val));

    if(!head)
        head = tmp;
    else
        tail->next = tmp;

    tail = tmp;
    ++size;
}

template <typename Data>
void List<Data>::RemoveFromBack() // (must throw std::length_error when empty)
{
    if(!head)
        throw std::length_error("Empty Exception from List\n");

    if(head == tail)
    {
        delete head;
        head = tail = nullptr;
    }
    else
    {
        Node * slow_ptr = head;
        while(slow_ptr && slow_ptr->next != tail)
            slow_ptr = slow_ptr->next;
        
        delete tail;
        tail = slow_ptr;
        slow_ptr->next = nullptr; 
    }

    --size;
}

template <typename Data>
Data List<Data>::BackNRemove() // (must throw std::length_error when empty)
{
    if(!head)
        throw std::length_error("Empty Exception from List\n");
    
    Data val(std::move(tail->elem)); 
    RemoveFromBack();

    return val; 
}

/* ---------------------------List: Member Functions (Mutable) -------------------------- */

template <typename Data>
Data & List<Data>::operator[](const ulong index) // Override MutableLinearContainer member (must throw std::out_of_range when out of range)
{
   return const_cast<Data &>(static_cast<const List<Data> *> (this)->operator[](index));
}

template <typename Data>
inline Data & List<Data>::Front() // Override MutableLinearContainer member (must throw std::length_error when empty)
{
    return const_cast<Data &>(static_cast<const List<Data> *> (this)->Front());
}

template <typename Data>
inline Data & List<Data>::Back() // Override MutableLinearContainer member (must throw std::length_error when empty)
{
    return const_cast<Data &>(static_cast<const List<Data> *> (this)->Back());
}

/* ---------------------------List: Member Functions (Linear) -------------------------- */

template <typename Data>
const Data & List<Data>::operator[](const ulong index) const // Override LinearContainer member (must throw std::out_of_range when out of range)
{
    if(index >= size)
         throw std::out_of_range("Out Of Range Exception from List\n");
    
    Node * walk_ptr = head; 

    for(ulong i = 0; i < index; i++, walk_ptr = walk_ptr->next);
    
    return walk_ptr->elem;
}

template <typename Data>
inline const Data & List<Data>::Front() const // Override LinearContainer member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from List\n");

    return head->elem;
}

template <typename Data>
inline const Data & List<Data>::Back() const // Override LinearContainer member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from List\n");
    
    return tail->elem;
}

/* ---------------------------List: Member Functions (Mappable) -------------------------- */

template <typename Data>
inline void List<Data>::Map(MapFun fun)
{
    PreOrderMap(fun);
}

/* ---------------------------List: Member Functions (PreOrderMappable) -------------------------- */

template <typename Data>
void List<Data>::PreOrderMap(MapFun fun)
{
    Node * walk_ptr = head;

    while(walk_ptr)
    {
        fun( walk_ptr->elem );
        walk_ptr = walk_ptr->next; 
    }
}

/* ---------------------------List: Member Functions (PostOrderMappable) -------------------------- */

template <typename Data>
inline void List<Data>::PostOrderMap(MapFun fun)
{
    PostOrderMap(fun, head);
}

/* ---------------------------List: Member Functions (Traversable) -------------------------- */

template <typename Data>
inline void List<Data>::Traverse(TraverseFun fun) const
{
    PreOrderTraverse(fun);
}

/* ---------------------------List: Member Functions (PreOrderTraversable) -------------------------- */

template <typename Data>
void List<Data>::PreOrderTraverse(TraverseFun fun) const
{
    Node * walk_ptr = head;

    while(walk_ptr)
    {
        fun( walk_ptr->elem );
        walk_ptr = walk_ptr->next; 
    }
}

/* ---------------------------List: Member Functions (PostOrderTraversable) -------------------------- */

template <typename Data>
inline void List<Data>::PostOrderTraverse(TraverseFun fun) const
{
    PostOrderTraverse(fun, head);
}

/* ---------------------------List: Member Functions (Clearable) -------------------------- */

template <typename Data>
void List<Data>::Clear()
{
    delete head;
    size = 0;
    head = tail = nullptr;
}

/* ---------------------------List: Auxiliary functions (To implement Recursive PostOrder Map/Traverse) -------------------------- */

template <typename Data>
void List<Data>::PostOrderMap(MapFun fun, Node *curr)
{
    if(curr)
        {
            PostOrderMap(fun, curr->next);
            fun(curr->elem);
        }
}

template <typename Data>
void List<Data>::PostOrderTraverse(TraverseFun fun, const Node *curr) const
{
    if(curr)
        {
            PostOrderTraverse(fun, curr->next);
            fun(curr->elem);
        }
}

/* ************************************************************************** */

}