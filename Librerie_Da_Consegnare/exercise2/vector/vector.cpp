// #include <cmath>
#include "vector.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------Vector: Specific Constructors/Destructors-------------------------- */

// A vector with a given initial dimension
template <typename Data>
Vector<Data>::Vector(const ulong dim)
{
    size = dim;
    Elements = new Data[size] {};
}

// A vector obtained from a TraversableContainer
template <typename Data>
Vector<Data>::Vector(const TraversableContainer<Data> &cont) : Vector(cont.Size())
{
    ulong i = 0;
    cont.Traverse
    (
        [this, &i] (const Data & curr)
        {
            Elements[i++] = curr;
        }
    );
}

// A vector obtained from a MappableContainer
template <typename Data>
Vector<Data>::Vector(MappableContainer<Data> &&cont) : Vector(cont.Size())
{
    ulong i = 0;
    cont.Map
    (
        [this, &i] (Data & curr)
        {
            Elements[i++] = std::move(curr);
        }
    );
}

// Copy Constructor
template <typename Data>
Vector<Data>::Vector(const Vector<Data> & vec) : Vector(vec.size)
{
    for(ulong i = 0; i < size; i++)
        Elements[i] = vec[i];
}

// Move Constructor
template <typename Data>
Vector<Data>::Vector(Vector<Data> && vec) noexcept
{
    std::swap(size, vec.size);
    std::swap(Elements, vec.Elements);
}

// Destructor
template <typename Data>
Vector<Data>::~Vector()
{
    delete[] Elements;
}

/* ---------------------------Vector: Assignments-------------------------- */

// Copy assignment
template <typename Data>
Vector<Data> & Vector<Data>::operator=(const Vector<Data> & vec)
{
    Vector<Data> * tmp = new Vector<Data>(vec);

    std::swap(*tmp, *this);
    delete tmp;

    return *this;
}

// Move assignment
template <typename Data>
Vector<Data> & Vector<Data>::operator=(Vector<Data> && vec) noexcept
{
    std::swap(size, vec.size);
    std::swap(Elements, vec.Elements);

    return *this;
} 

/* ---------------------------Vector: Comparison Operators-------------------------- */

template <typename Data>
bool Vector<Data>::operator==(const Vector<Data> & vec) const noexcept
{
    if(size != vec.size)
        return false;

    for(ulong i = 0; i < size; i++)
        if(Elements[i] != vec[i])
            return false;
    
    return true;
}

template <typename Data>
inline bool Vector<Data>::operator!=(const Vector<Data> & vec) const noexcept
{
    return !( (*this) == vec );
}

/* ---------------------------Vector: Comparison Operators (float, double)-------------------------- */

/*
template <>
inline bool Vector<float>::operator==(const Vector<float> &vec) const noexcept // da controllare inline
{
    if(size != vec.size)
        return false;

    constexpr float epsilon = 1e-5F;
    for(ulong i = 0; i < size; i++)
        if( fabs(Elements[i] - vec[i]) > epsilon)
            return false;
    
    return true;
}

template <>
inline bool Vector<double>::operator==(const Vector<double> &vec) const noexcept
{
    if(size != vec.size)
        return false;

    constexpr double epsilon = 1e-9;
    for(ulong i = 0; i < size; i++)
        if( fabs(Elements[i] - vec[i]) > epsilon)
            return false;
    
    return true;
}
*/

/* ---------------------------Vector: Member Functions (Mutable) -------------------------- */

template <typename Data>
inline Data & Vector<Data>::operator[](const ulong index) // Override MutableLinearContainer member (must throw std::out_of_range when out of range)
{
     return const_cast<Data &>(static_cast<const Vector<Data> *> (this)->operator[](index));
}

template <typename Data>
inline Data & Vector<Data>::Front() // Override MutableLinearContainer member (must throw std::length_error when empty)
{
     return const_cast<Data &>(static_cast<const Vector<Data> *> (this)->Front());
}

template <typename Data>
inline Data & Vector<Data>::Back()  // Override MutableLinearContainer member (must throw std::length_error when empty)
{
    return const_cast<Data &>(static_cast<const Vector<Data> *> (this)->Back());
}

/* ---------------------------Vector: Member Functions (Linear) -------------------------- */

template <typename Data>
inline const Data & Vector<Data>::operator[](const ulong index) const // Override LinearContainer member (must throw std::out_of_range when out of range)
{
    if(index >= size)
        throw std::out_of_range("Out Of Range Exception from LinearContainer(Vector)\n");
    return Elements[index];    
}

template <typename Data>
inline const Data & Vector<Data>::Front() const // Override LinearContainer member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Length Error Exception from LinearContainer(Vector): It is Empty\n"); 
    
    return Elements[0];
}

template <typename Data>
inline const Data & Vector<Data>::Back() const // Override LinearContainer member (must throw std::length_error when empty)
{
    if(size == 0)
        throw std::length_error("Length Error Exception from LinearContainer(Vector): It is Empty\n");
    
    return Elements[size - 1];
}

/* ---------------------------Vector: Member Functions (Resize) -------------------------- */

template <typename Data>
void Vector<Data>::Resize(const ulong dim)
{
    if(dim == 0)
    {
        Clear();
    }
    else if(dim != size)
    {
        Data * newElements = new Data[dim] {};
        ulong commonSize = (dim < size) ? dim : size;

        for(ulong i = 0; i < commonSize; i++)
            std::swap(Elements[i], newElements[i]);

        std::swap(Elements, newElements);
        size = dim;
        delete[] newElements;
    }    
}

/* ---------------------------Vector: Member Functions (Clearable) -------------------------- */

template <typename Data>
void Vector<Data>::Clear()
{
    delete[] Elements;
    Elements = nullptr;
    size = 0;
}

/* ---------------------------SortableVector: Constructors/Destructors-------------------------- */

// A vector with a given initial dimension
template <typename Data>
SortableVector<Data>::SortableVector(const ulong dim) : Vector<Data>(dim) {}

// A vector obtained from a TraversableContainer
template <typename Data>
SortableVector<Data>::SortableVector(const TraversableContainer<Data> & cont) : Vector<Data>(cont) {}

// A vector obtained from a MappableContainer
template <typename Data>
SortableVector<Data>::SortableVector(MappableContainer<Data> && cont) : Vector<Data>(std::move(cont)) {}

// Copy Constructor
template <typename Data>
SortableVector<Data>::SortableVector(const SortableVector<Data> & vec) : Vector<Data>(vec) {}

// Move Constructor
template <typename Data>
SortableVector<Data>::SortableVector(SortableVector<Data> && vec) noexcept : Vector<Data>(std::move(vec)) {}

/* ---------------------------SortableVector: Assignments-------------------------- */

// Copy assignment
template <typename Data>
SortableVector<Data> & SortableVector<Data>::operator=(const SortableVector<Data> &sortVec)
{
    Vector<Data>::operator=(sortVec);
    return *this;
}

// Move assignment
template <typename Data>
SortableVector<Data> & SortableVector<Data>::operator=(SortableVector<Data> &&sortVec) noexcept
{
    Vector<Data>::operator=(std::move(sortVec));
    return *this;
}

/* ************************************************************************** */

}