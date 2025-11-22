#include "pqheap.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------PQHeap: Constructors/Destructors -------------------------- */

template <typename Data>
PQHeap<Data>::PQHeap() : Vector<Data>(2)
{
    capacity = size;
    size = 0;   
}

// A priority queue obtained from a TraversableContainer 
template <typename Data>
PQHeap<Data>::PQHeap(const TraversableContainer<Data> &cont) : Vector<Data>::Vector(cont), HeapVec<Data>::HeapVec(cont)
{
    capacity = size;

    if(size < 2)
        Resize(2);
}

// A priority queue obtained from a MappableContainer
template <typename Data>
PQHeap<Data>::PQHeap(MappableContainer<Data> &&cont) : Vector<Data>::Vector(std::move(cont))
{
    capacity = size; 

    Heapify();

    if(size < 2)
        Resize(2);
} 

// Copy constructor
template <typename Data>
PQHeap<Data>::PQHeap(const PQHeap &cpheap) : Vector<Data>::Vector(cpheap), SortableVector<Data>::SortableVector(cpheap), HeapVec<Data>::HeapVec(cpheap)
{   
    capacity = size;
}

// Move constructor
template <typename Data>
PQHeap<Data>::PQHeap(PQHeap &&mvheap) noexcept : Vector<Data>::Vector(std::move(mvheap))
{
    std::swap(this->capacity, mvheap.capacity);
}

/* ---------------------------PQHeap: Assignments -------------------------- */

// Copy assignment
template <typename Data>
PQHeap<Data> & PQHeap<Data>::operator=(const PQHeap &cpheap)
{
    PQHeap<Data> * tmp = new PQHeap<Data>(cpheap);

    std::swap(*tmp, *this);
    delete tmp;

    return *this;
}

// Move assignment
template <typename Data>
PQHeap<Data> & PQHeap<Data>::operator=(PQHeap &&mvheap) noexcept
{
    HeapVec<Data>::operator=(std::move(mvheap));
    std::swap(this->capacity, mvheap.capacity);

    return *this;
}

/* ---------------------------PQHeap: Specific member functions (inherited from PQ) -------------------------- */

template <typename Data>
inline const Data & PQHeap<Data>::Tip() const // Override PQ member (must throw std::length_error when empty)
{
    return Front();
}

template <typename Data>
void PQHeap<Data>::RemoveTip() // Override PQ member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Length Exception: PQHeap is empty");

    std::swap((*this)[0], (*this)[size - 1]);
    HeapifyDown(--size, 0);

    checkResize();
}

template <typename Data>
Data PQHeap<Data>::TipNRemove() // Override PQ member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Length Exception: PQHeap is empty");

    Data tmpTip = std::move((*this)[0]);

    RemoveTip();

    return tmpTip;
}

template <typename Data>
void PQHeap<Data>::Insert(const Data &cpheap) // Override PQ member (Copy of the value)
{
    checkResize();

    Elements[size++] = cpheap;

    HeapifyUp(size - 1); //HeapifyUp
}

template <typename Data>
void PQHeap<Data>::Insert(Data &&mvheap) // Override PQ member (Move of the value)
{
    checkResize();

    Elements[size++] = std::move(mvheap);

    HeapifyUp(size - 1); //HeapifyUp
}

template <typename Data>
void PQHeap<Data>::Change(ulong index, const Data &cpdata) // Override PQ member (Copy of the value)
{
    Data oldValue = (*this)[index];
    (*this)[index] = cpdata;

    if(cpdata > oldValue)
        HeapifyUp(index);
    else if(cpdata < oldValue)
        HeapifyDown(size, index);
}

template <typename Data>
void PQHeap<Data>::Change(ulong index, Data &&mvdata)  // Override PQ member (Move of the value)
{
    Data oldValue = (*this)[index];
    (*this)[index] = std::move(mvdata);

    if(mvdata > oldValue)
        HeapifyUp(index);
    else if(mvdata < oldValue)
        HeapifyDown(size, index);
}

/* ---------------------------PQHeap: Auxilary Functions -------------------------- */

template <typename Data>
void PQHeap<Data>::HeapifyUp(ulong index) noexcept                                                        
{
    if(index > 0)
    {
        ulong dad = (index - 1) / 2;
        if(Elements[dad] < Elements[index])
        {
            std::swap(Elements[dad], Elements[index]);
            HeapifyUp(dad);
        }
    }    
}

template <typename Data>
void PQHeap<Data>::checkResize()
{
    if(capacity < 2) 
    {
        delete[] Elements;

        Elements = new Data[2] {};
        capacity = 2;
        size = 0;
    }
    else if(static_cast<double>(size) / capacity <= 0.25 && capacity > 2)
        Resize(std::max(capacity / 2, 2UL));
    else if(static_cast<double>(size) / capacity >= 0.9)
        Resize(capacity * 2);
}

template <typename Data>
void PQHeap<Data>::Resize(const ulong newCapacity)
{
    Data * resElements = new Data[newCapacity] {};
    
    for(ulong i = 0; i < size; i++)
        resElements[i] = (*this)[i];

    std::swap(Elements, resElements);

    this->capacity = newCapacity;

    delete[] resElements;
    resElements = nullptr;
}

template <typename Data>
void PQHeap<Data>::Clear()
{
    delete[] Elements;
    Elements = new Data[2] {};
    capacity = 2;
    size = 0;
}

/* ************************************************************************** */

}
