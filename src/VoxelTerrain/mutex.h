#ifndef MUTEX_H
#define MUTEX_H

#include "boost/thread/locks.hpp"
#include "boost/signals2/mutex.hpp"

namespace utility {

class mutex : public boost::signals2::mutex
{
public:
/*#ifdef BLUB_LOG_MUTEX
    mutex(string desc, bool debug = true)
        : m_desc(desc)
        , m_debug(debug)
    {
    }

    void lock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("lock(): " + m_desc);
        boost::mutex::lock();
    }

    void unlock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("unlock(): " + m_desc);
        boost::mutex::unlock();
    }
    bool tryLock(bool deb = true)
    {
        if (deb && m_debug)
            BOUT("tryLock(): " + m_desc);
        return try_lock();
    }
#else*/
    mutex()
    {
    }
    void lock()
    {
        boost::signals2::mutex::lock();
    }
    void unlock()
    {
        boost::signals2::mutex::unlock();
    }
    bool tryLock()
    {
        return try_lock();
    }
/*#endif



protected:
#ifdef BLUB_LOG_MUTEX
    string m_desc;
    bool m_debug;
#endif*/
};

}

#endif // MUTEX_H
