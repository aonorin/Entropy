#ifndef PLAYERACTIONGUI_H
#define PLAYERACTIONGUI_H

#include "src/Menu/menu.h"
#include "gameplay/abilities/ability.h"

namespace gui
{

class PlayerActionGui : public Gui
{
public:
    PlayerActionGui(const Ogre::Vector2& min, const Ogre::Vector2& max);

    void setActionSlot(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext,unsigned int slot, const std::string& name);
    const std::string& getActionSlot(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext, unsigned int slot) const;

    void setContext(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext);

    void buttonPressed(ActionWidget* actionButton);

    void keyPressed(const OIS::KeyEvent &arg);
    static const unsigned int NumActions = 4;

protected:
    neuro::Ability::Type hackContext;
    neuro::Ability::SubType subContext;
    typedef std::map<neuro::Ability::Type, std::map< neuro::Ability::SubType, std::vector< std::string > > > ActionMap;
    ActionMap actions;
};

} // namespace

#endif // PLAYERACTIONGUI_H
