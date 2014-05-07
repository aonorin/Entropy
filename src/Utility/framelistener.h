#ifndef FRAMELISTENER_H
#define FRAMELISTENER_H

#include "boost/signals2.hpp"
#include "boost/asio.hpp"
#include "boost/chrono/chrono.hpp"
#include "boost/thread.hpp"

namespace utility {

class FrameListener
{
public:
    FrameListener()
        : m_workToDo(m_toDo)
        , m_workToDoAfter(m_toDoAfter)
        , m_stop(false)
    {
    }
    ~FrameListener()
    {
        m_stop = true;
        m_toDo.stop();
        m_toDoAfter.stop();
        if (m_thread.joinable())
            m_thread.join();
    }

    typedef boost::signals2::signal<bool (double)> t_sigFrameListener;
    boost::signals2::connection addFrameListener(const t_sigFrameListener::slot_type &subscriber)
    {
        return m_sigFrame.connect(subscriber);
    }
    template<typename CompletionHandler>
    void addToDo(CompletionHandler handler)
    {
        m_toDo.post(handler);
    }
    template<typename CompletionHandler>
    void addToDoAfter(CompletionHandler handler)
    {
        m_toDoAfter.post(handler);
    }

    void run(double frames = 60.0, bool threaded = false)
    {
        m_frames = frames;
        if (threaded)
        {
            m_thread = boost::thread(boost::bind(&FrameListener::run, this, frames, false));
            return;
        }
        double delta = 0.01;
        while(!m_stop)
        {
            boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
            frame(1.0/frames);
            boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
            delta = sec.count();
            if (delta < 1.0/frames) {
                boost::this_thread::sleep(boost::posix_time::milliseconds(((1.0/frames)-delta) *1000000.0));
            }
                /*
#ifdef BLUB_WIN
                Sleep( ((1.0/frames)-delta) *1000.0);
#else
                usleep( ((1.0/frames)-delta) *1000000.0);
#endif
*/
        }
    }
protected:
    bool frame(double delta)
    {
        // boost::chrono::system_clock::time_point start = boost::chrono::system_clock::now();
        /*while (true)
        {
            boost::chrono::duration<double> sec = boost::chrono::system_clock::now() - start;
            if (sec.count() > 1.0 / m_frames)
                break;
            if (m_toDo.poll_one() == 0)
                break;
        }*/
        try {
            m_toDo.poll();
        } catch(std::exception e) {
            std::cerr << "An exception occured: " << e.what() << std::endl;
        }

        try {
            m_toDoAfter.poll();
        } catch(std::exception e) {
            std::cerr << "An exception occured: " << e.what() << std::endl;
        }

        try {
            m_sigFrame(delta);
        } catch(std::exception e) {
            std::cerr << "An exception occured: " << e.what() << std::endl;
        }
        return true;
    }
    /*bool frameOneToDo(real delta)
    {
        m_toDo.poll_one();
        m_toDoAfter.poll();
        m_sigFrame(delta);
        return true;
    }*/
private:

    t_sigFrameListener m_sigFrame;
    boost::asio::io_service m_toDo;
    boost::asio::io_service::work m_workToDo;
    boost::asio::io_service m_toDoAfter;
    boost::asio::io_service::work m_workToDoAfter;
    boost::thread m_thread;
    double m_frames;
    bool m_stop;
};

}

#endif // FRAMELISTENER_H
