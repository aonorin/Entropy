#ifndef OCTREE_H
#define OCTREE_H

#include "Core/global.h"

#ifdef __linux
typedef unsigned short uint16;
#endif
namespace utility {

template <typename dataType>
class Octree
{
public:
    Octree()
    {
        for (entropy::uint16 ind = 0; ind < 8; ++ind)
            m_children[ind] = 0;
    }

    Octree<dataType> **child(const entropy::uint16 ind)
    {return &m_children[ind];}
    void child(const entropy::uint16 ind, Octree<dataType>* tree)
    {m_children[ind] = tree;}

    dataType data()
    {return m_data;}
    void data(dataType dT)
    {m_data = dT;}

private:
    Octree<dataType> *m_children[8];
    dataType m_data;
};

}

#endif // OCTREE_H
