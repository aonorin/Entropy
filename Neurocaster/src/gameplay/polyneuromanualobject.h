#ifndef POLYNEUROMANUALOBJECT_H
#define POLYNEUROMANUALOBJECT_H

#include <map>

#include "neuron.h"

namespace neuro
{

class PolyNeuroManualObject
{
public:
//    typedef boost::shared_ptr<PolyNeuroManualObject> Ptr;
//    typedef std::vector<Ptr> PtrArray;
    PolyNeuroManualObject();

    void init_manuals();
    void async_init_manuals();
    virtual void create_manuals() = 0;

    void show_manual(std::string manual);
    void hide_manual(std::string manual);

//    Square getPos();
    void setPos(Square pos);

protected:
//    Square pos;
    //Organized via name
    typedef std::map<std::string,NeuroManualObject::Ptr> NeuroMap;
    NeuroMap neuro_manuals;


};

}

#endif // POLYNEUROMANUALOBJECT_H
