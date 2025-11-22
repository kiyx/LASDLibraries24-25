#include "setvec.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------SetVec: Specific Constructors/Destructors-------------------------- */

// Specific constructors
template <typename Data>
SetVec<Data>::SetVec(const TraversableContainer<Data> &cont) : SetVec<Data>(cont.Size()) // A set obtained from a TraversableContainer
{
    cont.Traverse
    (
        [this] (const Data &curr)
        {   
            Insert(curr);
        }
    );
}

template <typename Data>
SetVec<Data>::SetVec(MappableContainer<Data> &&cont) : SetVec<Data>(cont.Size()) // A set obtained from a MappableContainer
{
    cont.Map
    (
        [this] (Data &curr)
        {
            Insert(std::move(curr));
        }
    );
}

// Copy constructor
template <typename Data>
SetVec<Data>::SetVec(const SetVec &vec) : SetVec<Data>(vec.capacity)
    {
        this->size = vec.size;
        this->head = vec.head;
        
        for(ulong i = 0; i < size; i++)
            (*this)[i] = vec[i];
    }

// Move constructor
template <typename Data>
SetVec<Data>::SetVec(SetVec &&vec) noexcept
{
    std::swap(this->Elements, vec.Elements);
    std::swap(this->capacity, vec.capacity);
    std::swap(this->size, vec.size);
    std::swap(this->head, vec.head);
}

/* ---------------------------SetVec: Assignments-------------------------- */

// Copy assignment
template <typename Data>
SetVec<Data> & SetVec<Data>::operator=(const SetVec &vec)
{
    SetVec<Data> * tmp = new SetVec<Data>(vec);

    std::swap(*tmp, *this);
    delete tmp;

    return *this;
}

// Move assignment
template <typename Data>
SetVec<Data> & SetVec<Data>::operator=(SetVec &&vec) noexcept
{
    std::swap(this->Elements, vec.Elements);
    std::swap(this->capacity, vec.capacity);
    std::swap(this->size, vec.size);
    std::swap(this->head, vec.head);

    return *this;
}

/* ---------------------------SetVec: Comparison Operators-------------------------- */

template <typename Data>
bool SetVec<Data>::operator==(const SetVec &vec) const noexcept
{
    if(this->size == vec.size)
    {
        for(ulong i = 0; i < this->size ; i++)
            if( (*this)[i] != vec[i])
                return false;
        return true;
    }
    return false;
}

template <typename Data>
inline bool SetVec<Data>::operator!=(const SetVec &vec) const noexcept
{
    return !((*this) == vec);
}

/* ---------------------------SetVec: Specific member functions (inherited from OrderedDictionaryContainer)------------------------- */

template <typename Data>
const Data & SetVec<Data>::Min() const // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");
    
    return (*this)[0];
}

template <typename Data>
Data SetVec<Data>::MinNRemove() // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");

    Data tmpMin = std::move((*this)[0]);

    head = (head + 1) % capacity; 
    size--;

    checkResize();
    
    return tmpMin;
}

template <typename Data>
void SetVec<Data>::RemoveMin() // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");

    head = (head + 1) % capacity;
    size--;
   
    checkResize();
}

template <typename Data>
const Data & SetVec<Data>::Max() const // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");
    
    return (*this)[size - 1];
}

template <typename Data>
Data SetVec<Data>::MaxNRemove() // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");

    Data tmpMax = std::move((*this)[size - 1]);

    if(size == 1)
        head = 0;

    size--;
    checkResize();

    return tmpMax;
}

template <typename Data>
void SetVec<Data>::RemoveMax() // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Empty Exception from SetVec\n");

    if(size == 1)
        head = 0;
    
    size--;
    checkResize();
}

template <typename Data>
const Data & SetVec<Data>::Predecessor(const Data &key) const // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchPred(key);

    if(index == size)
        throw std::length_error("Predecessor Not Found from SetVec");

    return (*this)[index];    
}

template <typename Data>
Data SetVec<Data>::PredecessorNRemove(const Data &key) // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchPred(key);

    if(index == size)
        throw std::length_error("Predecessor Not Found from SetVec");

    Data tmpPred = std::move((*this)[index]);

    IndexedRemove(index);

    return tmpPred;
}

template <typename Data>
void SetVec<Data>::RemovePredecessor(const Data &key) // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchPred(key);

    if(index == size)
        throw std::length_error("Predecessor Not Found from SetVec");

    IndexedRemove(index);
}

template <typename Data>
const Data & SetVec<Data>::Successor(const Data &key) const // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchSucc(key);

    if(index == size)
        throw std::length_error("Successor Not Found from SetVec");

    return (*this)[index];   
}

template <typename Data>
Data SetVec<Data>::SuccessorNRemove(const Data &key) // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchSucc(key);

    if(index == size)
        throw std::length_error("Successor Not Found from SetVec");

    Data tmpSucc = std::move((*this)[index]);

    IndexedRemove(index);

    return tmpSucc;
}

template <typename Data>
void SetVec<Data>::RemoveSuccessor(const Data &key) // Override OrderedDictionaryContainer member (concrete function must throw std::length_error when not found)
{
    ulong index = BSearchSucc(key);

    if(index == size)
        throw std::length_error("Successor Not Found from SetVec");

    IndexedRemove(index);
}

/* ---------------------------SetVec: Specific member functions (inherited from DictionaryContainer)------------------------- */

template <typename Data>
bool SetVec<Data>::Insert(const Data &key) // Override DictionaryContainer member (copy of the value)
{
    checkResize();
    
    ulong i = BSearchEqPred(key);
    
    if(i != size && (*this)[i] == key)
        return false;
    
    if(i == size)    // Devo inserire in testa prima di head (size == 0 o pred trovato)
    {
        head = (head == 0) ? capacity - 1 : head - 1; 
        Elements[head] = key;
    }
    else if (i == size - 1) // Devo inserire in coda dopo tail (binary va su size - 1)
    {
        Elements[(head + size) % capacity] = key;
    }
    else
    {
        ulong left_elems = i + 1;
        ulong right_elems = size - left_elems;

        if(left_elems >= right_elems)
        {

            RightShift(i + 1, right_elems);
            (*this)[i + 1] = key;
        } 
        else
        {
            LeftShift(0, left_elems);
            (*this)[i] = key;
            head = (head == 0) ? capacity - 1 : head - 1;
        }
    }

    size++;
    return true;
}

template <typename Data>
bool SetVec<Data>::Insert(Data &&key) // Override DictionaryContainer member (move of the value)
{
    checkResize();
    
    ulong i = BSearchEqPred(key);
    
    if(i != size && (*this)[i] == key)
        return false;
    
    if(i == size)    // Devo inserire in testa prima di head (size == 0 o pred trovato)
    {
        head = (head == 0) ? capacity - 1 : head - 1; 
        Elements[head] = std::move(key);
    }
    else if (i == size - 1) // Devo inserire in coda dopo tail (binary va su size - 1)
    {
        Elements[(head + size) % capacity] = std::move(key);
    }
    else
    {
        ulong left_elems = i + 1;
        ulong right_elems = size - left_elems;

        if(left_elems >= right_elems)
        {
            RightShift(i + 1, right_elems);
            (*this)[i + 1] = std::move(key);
        } 
        else
        {
            LeftShift(0, left_elems);
            (*this)[i] = std::move(key);
            head = (head == 0) ? capacity - 1 : head - 1;
        }
    }

    size++;
    return true;
}

template <typename Data>
bool SetVec<Data>::Remove(const Data &key) // Override DictionaryContainer member
{   
    ulong i = BSearchExists(key);
    
    if(i == size)
        return false;

    IndexedRemove(i);

    return true;
}

/* ---------------------------SetVec: Specific member functions (inherited from LinearContainer)------------------------- */

template <typename Data>
inline const Data & SetVec<Data>::operator[](const ulong offset) const
{
    if(offset >= size)
        throw std::out_of_range("Out Of Range Exception from LinearContainer(SetVec) \n");

    return Elements[(head + offset) % capacity];
}
/* ---------------------------SetVec: Specific member functions (inherited from TestableContainer)------------------------- */

template <typename Data>
bool SetVec<Data>::Exists(const Data &key) const noexcept
{
    return (BSearchExists(key) != size);
}

/* ---------------------------SetVec: Specific member functions (inherited from ClearableContainer)------------------------- */

template <typename Data>
void SetVec<Data>::Clear()
{
    delete[] Elements;
    Elements = new Data[2] {};
    capacity = 2;
    size = 0;
    head = 0;
}

/* ---------------------------SetVec: Protected auxilary functions------------------------- */

template<typename Data>
SetVec<Data>::SetVec(ulong newCapacity)
{
    Elements = new Data[newCapacity] {};
    capacity = newCapacity;
    size = 0;
    head = 0;
}

template <typename Data>
inline Data & SetVec<Data>::operator[](const ulong offset) 
{
    if(offset >= size)
        throw std::out_of_range("Out Of Range Exception from LinearContainer(SetVec) \n");

    return Elements[(head + offset) % capacity];
}

template<typename Data>
void SetVec<Data>::RightShift(ulong index, ulong to_shift)
{
    if(index + to_shift == size)
    {
        Elements[(index + head + to_shift) % capacity] = Elements[(index + head + to_shift + capacity - 1) % capacity];
        --to_shift;
    }
    for(ulong i = to_shift; i > 0; i--)
        (*this)[index + i] = (*this)[index + i - 1];
}

template<typename Data>
void SetVec<Data>::LeftShift(ulong index, ulong to_shift)
{
    for(ulong i = 0 ; i < to_shift; i++)
        Elements[(index + i + head + capacity - 1) % capacity] = 
        Elements[(index + i + head) % capacity];
}

template <typename Data>
void SetVec<Data>::IndexedRemove(ulong to_remove) // Quando ho già la posizione
{
    ulong left_elems = to_remove;
    ulong right_elems = size - to_remove - 1;

    if(left_elems >= right_elems)
    {
        if(right_elems > 0)
            LeftShift(to_remove + 1, right_elems);
    }
    else
    {
        if(left_elems > 0)
            RightShift(0, left_elems);
        
        head = (head + 1) % capacity;
    }

    size--;

    if(size == 0) head = 0;

    checkResize();
}

template <typename Data>
void SetVec<Data>::checkResize()
{ 
    if(capacity < 2) 
    {
        delete[] Elements;

        Elements = new Data[2] {};
        capacity = 2;
        size = 0;
        head = 0;
    }
    else if(static_cast<double>(size) / capacity <= 0.25 && capacity > 2)
        Resize(std::max(capacity / 2, 2UL));
    else if(static_cast<double>(size) / capacity >= 0.9)
        Resize(capacity * 2);
}

// From ResizableContainer
template <typename Data>
void SetVec<Data>::Resize(ulong newCapacity)
{
    Data * resElements = new Data[newCapacity] {};

    ulong newHead = (newCapacity - size) / 2;
    
    for(ulong i = 0; i < size; i++)
        resElements[i + newHead] = (*this)[i];

    std::swap(Elements, resElements);

    this->capacity = newCapacity;
    this->head = newHead;

    delete[] resElements;
    resElements = nullptr;
}

// Ricerche specializzate per evitare confronti di uguaglianza nelle funzioni 

template <typename Data>
ulong SetVec<Data>::BSearchExists(const Data &key) const // Cerca == Key
{
    if(size == 0)
        return size;
    
    ulong i = 0;
    ulong j = size - 1;

    while(i <= j)
    {
        ulong q = i + (j - i) / 2;

        if((*this)[q] == key)
            return q; 
        if((*this)[q] < key)
            {
                i = q + 1;
            }
        else
        {
            if(q==0)
                break;
            
            j = q - 1;
        }
    }
    return size;
}

template<typename Data>
ulong SetVec<Data>::BSearchEqPred(const Data &key) const // Cerca <= key
{
    if(size == 0)
        return size;
    
    ulong i = 0;
    ulong j = size - 1;
    ulong pred_idx = size;

    while(i <= j)
    {
        ulong q = i + (j - i) / 2;

        if((*this)[q] == key)
            return q; 
        if((*this)[q] < key)
            {
                pred_idx = q;
                i = q + 1;
            }
        else
        {
            if(q==0)
                break;
            
            j = q - 1;
        }
    }
    return pred_idx;
}


template<typename Data>
ulong SetVec<Data>::BSearchPred(const Data &key) const
{
    if(size == 0)
        return size;

    ulong i = 0;
    ulong j = size - 1;
    ulong pred_idx = size;

    while (i <= j)
    {
        ulong q = i + (j - i) / 2;

        if ((*this)[q] < key)
        {
            pred_idx = q;
            i = q + 1;
        } 
        else
        {
            if(q == 0)
                break;

            j = q - 1;
        }
    }
    return pred_idx;
}

template<typename Data>
ulong SetVec<Data>::BSearchSucc(const Data &key) const
{
    if(size == 0)
        return size;

    ulong i = 0;
    ulong j = size - 1;
    ulong succ_idx = size;

    while (i <= j) 
    {
        ulong q = i + (j - i) / 2;

        if ((*this)[q] > key)
        {
            succ_idx = q;
            if (q == 0) 
                break;

            j = q - 1;
        } 
        else
        {
            i = q + 1;
        }
    }
    return succ_idx;
}

}
/* ************************************************************************** */



