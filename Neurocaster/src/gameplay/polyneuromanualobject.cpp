#include "polyneuromanualobject.h"
#include "boost/function.hpp"
#include "core/neurocaster.h"

using namespace neuro;

PolyNeuroManualObject::PolyNeuroManualObject()
{

}

void PolyNeuroManualObject::show_manual(std::string manual)
{

}

void PolyNeuroManualObject::hide_manual(std::string manual)
{

}

void PolyNeuroManualObject::async_init_manuals()
{
    create_manuals();
//    std::cout << "void PolyNeuroManualObject::async_init_manuals() : " << neuro_manuals.size() << std::endl;
    boost::function<void ()> func = boost::bind(&PolyNeuroManualObject::create_manuals,this);
    Neurocaster::getSingletonPtr()->register_async_func(func);
}

void PolyNeuroManualObject::init_manuals()
{
//    std::cout << neuro_manuals.size() << std::endl;
    for(NeuroMap::iterator it = neuro_manuals.begin();it != neuro_manuals.end(); ++it)
    {
//        std::cout << (*it).first << std::endl;
        (*it).second->init_manual();
    }
}

//Square PolyNeuroManualObject::getPos()
//{
//    return pos;
//}

void PolyNeuroManualObject::setPos(Square pos)
{
//    this->pos = pos;
    for(NeuroMap::iterator it = neuro_manuals.begin();it != neuro_manuals.end(); ++it)
    {
        (*it).second->setPos(pos);
    }
}
