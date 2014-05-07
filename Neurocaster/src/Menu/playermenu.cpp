#include <sstream>
#include "playermenu.h"
#include "src/core/neurocaster.h"

namespace gui
{

void buttonAction(ActionWidget* widget)
{
    std::cout << widget->getName() << " value: " << widget->getValue() << std::endl;
}

PlayerMenu::PlayerMenu(const Ogre::Vector2 &min, const Ogre::Vector2 &max) :
    Gui("PlayerMenu", min, max, VBOX),
    player(NULL)
{   
    View* top = addView("BottomView", HBOX); // The current gui layout is a VBOX and will automatically set the widget min/max
    View* bottom = addView("TopView", PROPORTIONAL);
    View* topLeft = top->addView("TopLeft", GRID); // Top is an HBOX and will automatically set the widget min/max
    View* topRight = top->addView("TopRight", HBOX);
    View* bottomLeft = bottom->addView("BottomLeft", HBOX, Ogre::Vector2(0.0, 0.0), Ogre::Vector2(0.3333, 1.0)); // BottomView is proportional, so we set min/max using ratios
    View* bottomRight = bottom->addView("BottomRight", VBOX, Ogre::Vector2(0.3333, 0.0), Ogre::Vector2(1.0, 1.0));

    for(unsigned int i = 0; i < 4; ++i)
    {
        std::stringstream ss;
        ss << i;
        bottomLeft->addButton(std::string(ss.str()).append("BL"), buttonAction);
        bottomRight->addButton(std::string(ss.str()).append("BR"), buttonAction);
        topLeft->addButton(std::string(ss.str()).append("TL"), buttonAction);
        topRight->addButton(std::string(ss.str()).append("TR"), buttonAction);
    }

    // ************************************************************
    rebuildView(); // ALWAYS CALL THIS WHEN DONE ADDING WIDGETS!!!
    // ************************************************************

    hide();
}

void PlayerMenu::show()
{
    player = Neurocaster::getSingletonPtr()->getCurrentMap()->getPlayer().get(); // we don't want to hold on to a smart pointer, just update the dumb pointer we have
    update();
    Gui::show();
}

void PlayerMenu::update()
{

}

} // gui namespace
