#ifndef MUTEXLOCKER_H
#define MUTEXLOCKER_H

#include "src/VoxelTerrain/mutex.h"

namespace utility {

class MutexLocker
{
public:
    MutexLocker(mutex& toLock)
        : m_mutex(toLock)
    {
        m_mutex.lock();
    }
    ~MutexLocker()
    {
        m_mutex.unlock();
    }
private:
    mutex &m_mutex;
};

}
#endif // MUTEXLOCKER_H
