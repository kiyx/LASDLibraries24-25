#include "linear.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------LinearContainer: Comparison Operators--------------------------- */

template <typename Data>
bool LinearContainer<Data>::operator==(const LinearContainer<Data> &cont) const noexcept
{
    if(size != cont.size)
        return false;

    for(ulong i = 0; i < size; i++)
        if( (*this)[i] != cont[i] )
            return false;
        
    return true;
}

template <typename Data>
inline bool LinearContainer<Data>::operator!=(const LinearContainer<Data> &cont) const noexcept
{
    return !(*this == cont);
}

/* ---------------------------LinearContainer: Member Functions--------------------------- */

template <typename Data>
inline const Data & LinearContainer<Data>::Front() const
{
    if(size == 0)
        throw std::length_error("Length Error Exception from LinearContainer(Linear): It is Empty\n");
    return (*this)[0];
}

template <typename Data>
inline const Data & LinearContainer<Data>::Back() const
{
    if(size == 0)
        throw std::length_error("Length Error Exception from LinearContainer(Linear): It is Empty\n");
    return (*this)[size - 1];
}

template <typename Data>
inline void LinearContainer<Data>::Traverse(TraverseFun fun) const
{
    PreOrderTraverse(fun);
}

template <typename Data>
inline void LinearContainer<Data>::PreOrderTraverse(TraverseFun fun) const
{
    for(ulong i = 0; i < size; i++)
        fun( (*this)[i] );
}

template <typename Data>
inline void LinearContainer<Data>::PostOrderTraverse(TraverseFun fun) const
{
    for(ulong i = size; i > 0; i--)
        fun( (*this)[i - 1] );
}


/* ---------------------------MutableLinearContainer: Member Functions--------------------------- */

template <typename Data>
inline Data & MutableLinearContainer<Data>::Front()
{
    if(size == 0)
        throw std::length_error("Length Error Exception from MutableLinearContainer(Linear): It is Empty\n");
    return (*this)[0];
}

template <typename Data>
inline Data & MutableLinearContainer<Data>::Back()
{
    if(size == 0)
        throw std::length_error("Length Error Exception from MutableLinearContainer(Linear): It is Empty\n");
    return (*this)[size - 1];
}

template <typename Data>
inline void MutableLinearContainer<Data>::Map(MapFun fun)
{
    PreOrderMap(fun);
}

template <typename Data>
inline void MutableLinearContainer<Data>::PreOrderMap(MapFun fun)
{
    for(ulong i = 0; i < size; i++)
        fun( (*this)[i] );
}

template <typename Data>
inline void MutableLinearContainer<Data>::PostOrderMap(MapFun fun)
{
    for(ulong i = size; i > 0; i--)
        fun( (*this)[i - 1] );
}

/* ---------------------------SortableLinearContainer: Member Functions--------------------------- */

template <typename Data>
inline void SortableLinearContainer<Data>::Sort() noexcept
{
    InsertionSort();
}

template <typename Data>
void SortableLinearContainer<Data>::InsertionSort() noexcept
{
    for(ulong i = 1; i < size; i++)
    {
        Data key = (*this)[i];
        ulong j = i;

        while( j > 0 && (*this)[j - 1] > key)
        {
            (*this)[j] = (*this)[j - 1];
            j--;
        }
        
        (*this)[j] = key;
    }
}

/* ************************************************************************** */

}
