#include <sstream>
#include "playeractiongui.h"
#include "src/core/neurocaster.h"
#include "src/graphics/abilityicongenerator.h"

namespace gui
{

PlayerActionGui::PlayerActionGui(const Ogre::Vector2 &min, const Ogre::Vector2 &max) :
    Gui("PlayerActionGui", min, max, HBOX),
    hackContext(neuro::Ability::CYBERNETICS),
    subContext(neuro::Ability::WHITE_HAT)
{   
    std::string label = "PlayerActionButton";

    for(unsigned int i = 0; i < PlayerActionGui::NumActions; ++i)
    {
        actions[neuro::Ability::CYBERNETICS][neuro::Ability::WHITE_HAT].push_back( neuro::generateAbilityName(neuro::Ability::CYBERNETICS) );
        actions[neuro::Ability::CYBERNETICS][neuro::Ability::BLACK_HAT].push_back( neuro::generateAbilityName(neuro::Ability::CYBERNETICS) );

        actions[neuro::Ability::CRYPTOGRAPHY][neuro::Ability::WHITE_HAT].push_back( neuro::generateAbilityName(neuro::Ability::CRYPTOGRAPHY) );
        actions[neuro::Ability::CRYPTOGRAPHY][neuro::Ability::BLACK_HAT].push_back( neuro::generateAbilityName(neuro::Ability::CRYPTOGRAPHY) );

        actions[neuro::Ability::INFECTION][neuro::Ability::WHITE_HAT].push_back( neuro::generateAbilityName(neuro::Ability::INFECTION) );
        actions[neuro::Ability::INFECTION][neuro::Ability::BLACK_HAT].push_back( neuro::generateAbilityName(neuro::Ability::INFECTION) );

        std::stringstream ss;
        ss << (i + 1); // offset to 1 - 6 for better keyboard layout
        Button* button = addButton(std::string(label).append(ss.str()), boost::bind(&PlayerActionGui::buttonPressed, this, _1), i);
        button->setLabel(ss.str());


    }

    // ************************************************************
    rebuildView(); // ALWAYS CALL THIS WHEN DONE ADDING WIDGETS!!!
    // ************************************************************

    show();
}

void PlayerActionGui::setActionSlot(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext, unsigned int slot, const std::string& name)
{
    if(slot < PlayerActionGui::NumActions)
        actions[hackContext][subContext][slot] = name;
}

const std::string& PlayerActionGui::getActionSlot(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext, unsigned int slot) const
{
    if(slot > PlayerActionGui::NumActions)
        slot = PlayerActionGui::NumActions - 1;

    return actions.at(hackContext).at(subContext).at(slot);
}

void PlayerActionGui::buttonPressed(ActionWidget* actionButton)
{
    Neurocaster::getSingletonPtr()->getCurrentMap()->getPlayer()->setActiveAbility(actions[hackContext][subContext][actionButton->getUserInfo().numberUI]);
}

void PlayerActionGui::keyPressed(const OIS::KeyEvent &arg)
{
    bool actionKeyPressed = false;
    std::string actionName;
    switch(arg.key)
    {
    case OIS::KC_1:
        actionName = actions[hackContext][subContext][0];
        actionKeyPressed = true;
        break;

    case OIS::KC_2:
        actionName = actions[hackContext][subContext][1];
        actionKeyPressed = true;
        break;

    case OIS::KC_3:
        actionName = actions[hackContext][subContext][2];
        actionKeyPressed = true;
        break;

    case OIS::KC_4:
        actionName = actions[hackContext][subContext][3];
        actionKeyPressed = true;
        break;

//    case OIS::KC_5:
//        actionName = actions[4];
//        actionKeyPressed = true;
//        break;

//    case OIS::KC_6:
//        actionName = actions[5];
//        actionKeyPressed = true;
//        break;

    default:
        break;
    }

    switch(hackContext)
    {
    case neuro::Ability::CYBERNETICS:
        std::cout << "CYBERNETICS NAME: ";
        break;
    case neuro::Ability::INFECTION:
        std::cout << "INFECTION NAME: ";
        break;
    case neuro::Ability::CRYPTOGRAPHY:
        std::cout << "CRYPTOGRAPHY NAME: ";
        break;
    }

    std::cout << actionName << std::endl;

    if(actionKeyPressed)
        Neurocaster::getSingletonPtr()->getCurrentMap()->getPlayer()->setActiveAbility(actionName);
}

void PlayerActionGui::setContext(neuro::Ability::Type hackContext, neuro::Ability::SubType subContext)
{
    this->hackContext = hackContext;
    this->subContext = subContext;
}

} // gui namespace
