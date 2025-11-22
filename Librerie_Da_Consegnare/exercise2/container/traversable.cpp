#include "traversable.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------TraversableContainer: Member Functions--------------------------- */

template <typename Data>
template <typename Accumulator>
inline Accumulator TraversableContainer<Data>::Fold(FoldFun<Accumulator> fun, Accumulator acc) const
{
    Traverse
            ( 
                [fun, &acc](const Data & curr)
                {
                    acc = fun(curr, acc);
                }
            );
    return acc;
}

template <typename Data>
inline bool TraversableContainer<Data>::Exists(const Data & val) const noexcept
{
    bool exists = false; 
    Traverse
    (
        [val, &exists](const Data & curr)
        {
           exists |= (curr == val);
        }
    );
    return exists;
}

/* ---------------------------PreOrderTraversableContainer: Member Functions--------------------------- */

template <typename Data>
template <typename Accumulator>
inline Accumulator PreOrderTraversableContainer<Data>::PreOrderFold(FoldFun<Accumulator> fun, Accumulator acc) const
{
    PreOrderTraverse
    (
        [fun, &acc](const Data & curr)
        {
            acc = fun(curr, acc);
        }
    );
    return acc;
}

template <typename Data>
inline void PreOrderTraversableContainer<Data>::Traverse(TraverseFun fun) const
{
    PreOrderTraverse(fun);
}

/* ---------------------------PostOrderTraversableContainer: Member Functions--------------------------- */

template <typename Data>
template <typename Accumulator>
inline Accumulator PostOrderTraversableContainer<Data>::PostOrderFold(FoldFun<Accumulator> fun, Accumulator acc) const
{
    PostOrderTraverse
    (
        [fun, &acc](const Data & curr)
        {
            acc = fun(curr, acc);
        }
    );
    return acc;
}

template <typename Data>
inline void PostOrderTraversableContainer<Data>::Traverse(TraverseFun fun) const
{
    PostOrderTraverse(fun);
}

/* ************************************************************************** */

}