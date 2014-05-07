#ifndef TODOLISTENER_H
#define TODOLISTENER_H

#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "Core/global.h"

//typedef unsigned short entropy::uint16;

class ToDoListener
{
public:
    ToDoListener(entropy::uint16 numThreads = 1)
        : m_work(m_toDo)
        , m_numThreads(numThreads)
    {
        if (m_numThreads > 0)
        {
            for (entropy::uint16 ind = 0; ind < m_numThreads; ++ind)
                m_threads.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_toDo)));
        }
    }

    ~ToDoListener()
    {
        m_toDo.stop();
        for (t_threads::iterator it = m_threads.begin(); it != m_threads.end(); ++it)
        {
            (*it)->join();
            delete (*it);
        }
    }

    template<typename CompletionHandler>
    void addToDo(CompletionHandler handler)
    {
        try{
            m_toDo.post(handler);
        } catch(std::exception e) {
            std::cerr << "An exception occured: " << e.what() << std::endl;
        }
    }

private:
    boost::asio::io_service m_toDo;
    boost::asio::io_service::work m_work;
    entropy::uint16 m_numThreads;
    typedef std::vector<boost::thread*> t_threads;
    t_threads m_threads;
    // boost::thread m_thread;
};

#endif // TODOLISTENER_H
