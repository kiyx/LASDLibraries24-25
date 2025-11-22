#include "dictionary.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------DictionaryContainer: Member Functions--------------------------- */

template <typename Data>
inline bool DictionaryContainer<Data>::InsertAll(const TraversableContainer<Data> & cont)
{
    bool check = true; 
    cont.Traverse
    (
        [this, &check](const Data &curr)
        {
            check &= Insert(curr);
        }
    );
    return check;
}

template <typename Data>
inline bool DictionaryContainer<Data>::InsertAll(MappableContainer<Data> && cont)
{
    bool check = true; 
    cont.Map
    (
        [this, &check](Data & curr)
        {
            check &= Insert(std::move(curr));
        }
    );
    return check;
}

template <typename Data>
inline bool DictionaryContainer<Data>::RemoveAll(const TraversableContainer<Data> & cont)
{
    bool check = true; 
    cont.Traverse
    (
        [this, &check](const Data & curr)
        {
            check &= Remove(curr);
        }
    );
    return check;
}

template <typename Data>
inline bool DictionaryContainer<Data>::InsertSome(const TraversableContainer<Data> & cont)
{
    bool check = false; 
    cont.Traverse
    (
        [this, &check](const Data & curr)
        {
            check |= Insert(curr);
        }
    );
    return check;
}

template <typename Data>
inline bool DictionaryContainer<Data>::InsertSome(MappableContainer<Data> && cont)
{
    bool check = false; 
    cont.Map
    (
        [this, &check](Data & curr)
        {
            check |= Insert(std::move(curr));
        }
    );
    return check;
}

template <typename Data>
inline bool DictionaryContainer<Data>::RemoveSome(const TraversableContainer<Data> & cont) 
{
    bool check = false; 
    cont.Traverse
    (
        [this, &check](const Data & curr)
        {
            check |= Remove(curr);
        }
    );
    return check;
}

/* ************************************************************************** */

}