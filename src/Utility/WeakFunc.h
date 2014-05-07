#ifndef WEAKFUNC_H
#define WEAKFUNC_H

//  weak_fun.hpp
//
//  Copyright (c) 2009 Artyom Beilis
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//

#include <boost/config.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace boost
{
namespace details {

#ifdef BOOST_HAS_VARIADIC_TMPL
    template<typename T,typename... Args>
    class weak_binder {
        typedef void (T::*member_type)(Args...);
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()(Args... args)
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)(args...);
            }
        }
    };
#else  // NO BOOST_HAS_VARIADIC_TMPL
    template<typename T>
    class weak_binder0 {
        typedef void (T::*member_type)();
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder0(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()()
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)();
            }
        }
    };

    template<typename T,typename P1>
    class weak_binder1 {
        typedef void (T::*member_type)(P1);
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder1(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()(P1 p1)
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)(p1);
            }
        }
    };

    template<typename T,typename P1,typename P2>
    class weak_binder2 {
        typedef void (T::*member_type)(P1,P2);
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder2(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()(P1 p1,P2 p2)
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)(p1,p2);
            }
        }
    };

    template<typename T,typename P1,typename P2,typename P3>
    class weak_binder3 {
        typedef void (T::*member_type)(P1,P2,P3);
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder3(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()(P1 p1,P2 p2,P3 p3)
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)(p1,p2,p3);
            }
        }
    };

    template<typename T,typename P1,typename P2,typename P3,typename P4>
    class weak_binder4 {
        typedef void (T::*member_type)(P1,P2,P3,P4);
        typedef boost::weak_ptr<T> pointer_type;
        typedef boost::shared_ptr<T> shared_type;
        member_type member_;
        pointer_type object_;
    public:
        typedef void result_type;
        weak_binder4(member_type member, pointer_type object) :
           member_(member),object_(object)
        {
        }
        void operator()(P1 p1,P2 p2,P3 p3,P4 p4)
        {
            shared_type object=object_.lock();
            if(object) {
                ((*object).*member_)(p1,p2,p3,p4);
            }
        }
    };
#endif // NO BOOST_HAS_VARIADIC_TMPL
} // namespace details

#ifdef BOOST_HAS_VARIADIC_TMPL

template<typename T,typename... Args>
details::weak_binder<T,Args...> weak_fn(void (T::*member)(Args...),boost::shared_ptr<T> p)
{
    return details::weak_binder<T,Args...>(member,p);
}

#else  // NO BOOST_HAS_VARIADIC_TMPL

template<typename T>
details::weak_binder0<T> weak_fn(void (T::*member)(),boost::shared_ptr<T> p)
{
    return details::weak_binder0<T>(member,p);
}

template<typename T,typename P1>
details::weak_binder1<T,P1> weak_fn(void (T::*member)(P1),boost::shared_ptr<T> p)
{
    return details::weak_binder1<T,P1>(member,p);
}

template<typename T,typename P1,typename P2>
details::weak_binder2<T,P1,P2> weak_fn(void (T::*member)(P1,P2),boost::shared_ptr<T> p)
{
    return details::weak_binder2<T,P1,P2>(member,p);
}

template<typename T,typename P1,typename P2,typename P3>
details::weak_binder3<T,P1,P2,P3> weak_fn(void (T::*member)(P1,P2,P3),boost::shared_ptr<T> p)
{
    return details::weak_binder3<T,P1,P2,P3>(member,p);
}

template<typename T,typename P1,typename P2,typename P3,typename P4>
details::weak_binder4<T,P1,P2,P3,P4> weak_fn(void (T::*member)(P1,P2,P3,P4),boost::shared_ptr<T> p)
{
    return details::weak_binder4<T,P1,P2,P3,P4>(member,p);
}

#endif // BOOST_HAS_VARIADIC_TMPL

} // end of namespace boost

#endif // WEAKFUNC_H
