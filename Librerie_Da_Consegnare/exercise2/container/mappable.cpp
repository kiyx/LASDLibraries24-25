#include "mappable.hpp"

namespace lasd {

/* ************************************************************************** */

/* ---------------------------PreOrderMappableContainer: Member Functions--------------------------- */

template <typename Data>
inline void PreOrderMappableContainer<Data>::Map(MapFun fun)
{
    PreOrderMap(fun);
}

/* ---------------------------PostOrderMappableContainer: Member Functions--------------------------- */

template <typename Data>
inline void PostOrderMappableContainer<Data>::Map(MapFun fun)
{
    PostOrderMap(fun);
}

/* ************************************************************************** */

}