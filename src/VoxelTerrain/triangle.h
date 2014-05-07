#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector"
#include "boost/shared_ptr.hpp"

namespace voxel {

struct Triangle
{
    typedef std::vector<Triangle> vector;
    typedef boost::shared_ptr<vector> vectorPtr;
    Triangle()
    {;}
    Triangle(unsigned int a, unsigned int b, unsigned int c)
    {
        ind[0] = a;
        ind[1] = b;
        ind[2] = c;
    }

    bool contains(unsigned int _ind)
    {
        return ind[0] == _ind || ind[1] == _ind || ind[2] == _ind;
    }

    bool valid()
    {
        return ind[0] != ind[1] && ind[0] != ind[2] && ind[1] != ind[2];
    }

    bool operator == (const Triangle & other) const
    {
        std::vector<unsigned int> otherVec;
        for (int ind1 = 0; ind1 < 3; ++ind1)
            otherVec.push_back(other.ind[ind1]);
        for (int ind1 = 0; ind1 < 3; ++ind1)
        {
            int index = -1;
            for(int i=0;i<otherVec.size();i++) {
                if(otherVec.at(i)==ind[ind1]) {
                    index = i;
                    break;
                }
            }
            if (index == -1) {
                return false;
            } else {
                otherVec.erase(otherVec.begin()+index);
            }
        }
        return true;
    }
    Triangle operator + (const int & other) const
    {
        return Triangle(ind[0]+other, ind[1]+other, ind[2]+other);
    }

    int ind[3];
};

}


#endif // TRIANGLE_H
