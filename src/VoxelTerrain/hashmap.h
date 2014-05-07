#ifndef HASHMAP_H
#define HASHMAP_H

#include "map.h"
#include "vector"

namespace voxel
{

template <class S, class T>
class hashMap : public std::map<S, T>
{
public:
    hashMap<S, T>() : std::map<S, T>() {;}

    void insertMultiList(S key, const std::vector<T> &toInsert)
    {
        for (typename std::vector<T>::const_iterator it = toInsert.constBegin(); it != toInsert.constEnd(); ++it)
        {
            //insertMulti(key, *it);
            insert(key,*it);
        }
    }

    /*
    void insert(S key, T object) {
            std::_Rb_tree_iterator<std::pair<const S, T> > it = find(key);
            std::map<S,T> test;
        if(it != this->end()) {
            erase(it);
            std::multimap<S,T>::insert(std::pair<S,T>(key,object));
        } else {
            std::multimap<S,T>::insert(std::pair<S,T>(key,object));
        }
    }
    */

};

}

#endif // HASHMAP_H
