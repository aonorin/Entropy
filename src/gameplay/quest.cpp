// Local includes
#include "quest.h"
#include "src/Script/script.h"
#include "src/Core/update.h"
#include "Menu/questmenu.h"

namespace gameplay {

/////////////////////////////////
// Quest Completed Action types
/////////////////////////////////

quest_args_t emptyArgs;

void defaultQuestAction(quest_args_t& arguments)
{

}


//////////////
// QuestItem
//////////////

QuestItem::QuestItem(std::string name, script::lua_functor_t completedAction, std::string description) :
    name(name),
    completedAction(completedAction),
    completed(false),
    description(description)
{

}

QuestItem::~QuestItem()
{

}

void QuestItem::setCompleted(bool completed)
{
    if(this->completed == false && completed == true)
    {
        //std::cout << "COMPLETED ACTION: " << completedAction << std::endl;
        //script::interpreter.callLuaFunction(completedAction, 0);
        std::cout << "QUEST ITEM BEGIN FUNC CALL" << std::endl;
        completedAction->call();
        std::cout << "QUEST ITEM FUNC CALL COMPLETED" << std::endl;
        //script::interpret(completedAction.c_str());
    }

    this->completed = completed;
}

bool QuestItem::getCompleted()
{
    return completed;
}

std::string QuestItem::getName()
{
    return name;
}

void QuestItem::setName(std::string name)
{
    this->name = name;
}

std::string QuestItem::getDescription()
{
    return description;
}

void QuestItem::setDescription(std::string description)
{
    this->description = description;
}

/////////
// Quest
/////////

Quest::Quest(std::string name, script::lua_functor_t completedAction, std::string description):
    name(name),
    completedAction(completedAction),
    completed(false),
    description(description)
{

}

Quest::~Quest()
{
    std::tr1::unordered_map<std::string, QuestItem*>::iterator iter = questItems.begin();
    while(iter != questItems.end())
    {
        delete iter->second;
        ++iter;
    }

    questItems.clear();
}

void Quest::addQuestItem(std::string itemName, script::lua_functor_t completedAction, std::string description)
{
    if(containsQuestItem(itemName))
    {
        std::cerr << "Quest " << name << ":  duplicate item named " << itemName
                  << "added. Discarding addition." << std::endl;
    }

    else
    {
        questItems[itemName] = new QuestItem(itemName, completedAction, description);
        update();
    }
}

void Quest::removeQuestItem(std::string itemName)
{
    delete questItems[name];
    questItems.erase(name);
    update();
}

void Quest::forceComplete()
{
    std::tr1::unordered_map<std::string, QuestItem*>::iterator iter = questItems.begin();
    while(iter != questItems.end())
    {
        iter->second->setCompleted(true);
        ++iter;
    }

    update();
}

void Quest::reset()
{
    std::tr1::unordered_map<std::string, QuestItem*>::iterator iter = questItems.begin();
    while(iter != questItems.end())
    {
        iter->second->setCompleted(false);
        ++iter;
    }

    update();
}

int Quest::numberOfQuestItems()
{
    return questItems.size();
}

void Quest::setQuestItemCompleted(std::string itemName, bool completed)
{
    if(containsQuestItem(itemName))
    {
        questItems[itemName]->setCompleted(completed);
        update();
    }
}

bool Quest::getQuestItemCompleted(std::string itemName)
{
    if(containsQuestItem(itemName))
        return questItems[itemName]->getCompleted();
    else
        return false;
}

bool Quest::getCompleted()
{
    return completed;
}

std::string Quest::getName()
{
    return name;
}

void Quest::setName(std::string name)
{
    this->name = name;
}

std::string Quest::getDescription()
{
    return description;
}

void Quest::setDescription(std::string description)
{
    this->description = description;
}

std::string Quest::getQuestItemDescription(std::string itemName)
{
    if(containsQuestItem(itemName))
        return questItems[itemName]->getDescription();
    else
        return "";
}

void Quest::setQuestItemDescription(std::string itemName, std::string description)
{
    if(containsQuestItem(itemName))
        return questItems[itemName]->setDescription(description);
}

bool Quest::containsQuestItem(std::string itemName)
{
    std::tr1::unordered_map<std::string, QuestItem*>::iterator find = questItems.find(itemName);
    if(find != questItems.end())
        return true;
    else
        return false;
}

bool Quest::update()
{
    if(numberOfQuestItems() == 0)
    {
        completed = false;
        return completed;
    }

    bool completionCheck = true;

    std::tr1::unordered_map<std::string, QuestItem*>::iterator iter = questItems.begin();
    while(iter != questItems.end())
    {
        if(iter->second->getCompleted() == false)
        {
            completed = false;
            return completed;
        }
        ++iter;
    }

    if(completionCheck == true && completed == false)
    {
        //std::cout << "COMPLETED ACTION: " << completedAction << std::endl;
        completedAction->call();
        //script::interpret(completedAction.c_str());
        MetaQuest::completeQuest(name);
    }

    completed = completionCheck;
    return completed;
}

std::tr1::unordered_map<std::string, QuestItem*>* Quest::getQuestItems()
{
    return &questItems;
}

/////////////
// MetaQuest
/////////////

quest_map_t MetaQuest::activeQuests;
quest_map_t MetaQuest::inactiveQuests;
quest_map_t MetaQuest::completedQuests;

MetaQuest::MetaQuest()
{
    std::cout << "INITIALIZING METAQUEST" << std::endl;
}

MetaQuest::~MetaQuest()
{
    MetaQuest::reset();
}

void MetaQuest::addQuest(Quest* quest, bool setActive)
{
    std::string name = quest->getName();
    if(MetaQuest::questInactive(name) || MetaQuest::questActive(name) || MetaQuest::questCompleted(name))
    {
        std::cerr << "MetaQuest: duplicate quest named " << name << "added. Discarding addition." << std::endl;
    }

    else
    {
        if(setActive) {
            MetaQuest::activeQuests[quest->getName()] = quest;
            QuestMenu::getSingletonPtr()->addQuestIcon(name,activeQuests[quest->getName()]->getDescription());
        } else {
            MetaQuest::inactiveQuests[quest->getName()] = quest;
        }
    }
}

void MetaQuest::addNewQuest(std::string questName, bool setActive, script::lua_functor_t completedAction, std::string description)
{
    if(MetaQuest::questInactive(questName) || MetaQuest::questActive(questName) || MetaQuest::questCompleted(questName))
    {
        std::cerr << "MetaQuest: duplicate quest named " << questName << "added. Discarding addition." << std::endl;
    }
    else
    {
        Quest* quest = new Quest(questName, completedAction, description);
        MetaQuest::addQuest(quest, setActive);
    }
}

void MetaQuest::addQuestItem(std::string questName, std::string questItemName, script::lua_functor_t completedAction,
                             std::string description)
{
    if(MetaQuest::questActive(questName)) {
        MetaQuest::activeQuests[questName]->addQuestItem(questItemName, completedAction, description);
        QuestMenu::getSingletonPtr()->addQuestItemIcon(questItemName,questName,description);
    } else if(MetaQuest::questInactive(questName)) {
        MetaQuest::inactiveQuests[questName]->addQuestItem(questItemName, completedAction, description);
    } else if(MetaQuest::questCompleted(questName)) {
        MetaQuest::completedQuests[questName]->addQuestItem(questItemName, completedAction, description);
    }
}

void MetaQuest::setQuestItemCompleted(std::string questName, std::string questItemName, bool completed)
{
    if(MetaQuest::questActive(questName)) {
        MetaQuest::activeQuests[questName]->setQuestItemCompleted(questItemName, completed);
        QuestMenu::getSingletonPtr()->removeQuestItemIcon(questItemName,questName);
    } else if(MetaQuest::questInactive(questName)) {
        MetaQuest::inactiveQuests[questName]->setQuestItemCompleted(questItemName, completed);
    } else if(MetaQuest::questCompleted(questName)) {
        MetaQuest::completedQuests[questName]->setQuestItemCompleted(questItemName, completed);
    }
}

std::string MetaQuest::getQuestDescription(std::string questName)
{
    if(MetaQuest::questActive(questName))
        return MetaQuest::activeQuests[questName]->getDescription();
    else if(MetaQuest::questInactive(questName))
        return MetaQuest::inactiveQuests[questName]->getDescription();
    else if(MetaQuest::questCompleted(questName))
        return MetaQuest::completedQuests[questName]->getDescription();
}

std::string MetaQuest::getQuestItemDescription(std::string questName, std::string questItemName)
{
    if(MetaQuest::questActive(questName))
        return MetaQuest::activeQuests[questName]->getQuestItemDescription(questItemName);
    else if(MetaQuest::questInactive(questName))
        return MetaQuest::inactiveQuests[questName]->getQuestItemDescription(questItemName);
    else if(MetaQuest::questCompleted(questName))
        return MetaQuest::completedQuests[questName]->getQuestItemDescription(questItemName);
}

void MetaQuest::setQuestDescription(std::string questName, std::string description)
{
    if(MetaQuest::questActive(questName))
        return MetaQuest::activeQuests[questName]->setDescription(description);
    else if(MetaQuest::questInactive(questName))
        return MetaQuest::inactiveQuests[questName]->setDescription(description);
    else if(MetaQuest::questCompleted(questName))
        return MetaQuest::completedQuests[questName]->setDescription(description);
}

void MetaQuest::setQuestItemDescription(std::string questName, std::string questItemName, std::string description)
{
    if(MetaQuest::questActive(questName))
        return MetaQuest::activeQuests[questName]->setQuestItemDescription(questItemName, description);
    else if(MetaQuest::questInactive(questName))
        return MetaQuest::inactiveQuests[questName]->setQuestItemDescription(questItemName, description);
    else if(MetaQuest::questCompleted(questName))
        return MetaQuest::completedQuests[questName]->setQuestItemDescription(questItemName, description);
}

void MetaQuest::activateQuest(std::string name)
{
    if(MetaQuest::questInactive(name))
    {
        MetaQuest::activeQuests[name] = MetaQuest::inactiveQuests[name];
        MetaQuest::inactiveQuests.erase(name);
        QuestMenu::getSingletonPtr()->addQuestIcon(name,MetaQuest::activeQuests[name]->getDescription());
        std::tr1::unordered_map<std::string, QuestItem*>* questItems = MetaQuest::activeQuests[name]->getQuestItems();
        std::tr1::unordered_map<std::string, QuestItem*>::iterator itemIter = questItems->begin();
        while(itemIter != questItems->end())
        {
            QuestMenu::getSingletonPtr()->addQuestItemIcon(
                        itemIter->second->getName(),name,itemIter->second->getDescription()
            );
            ++itemIter;
        }
        std::cout << "ACTIVATING QUEST: " <<  name << std::endl;
    }

    else if(MetaQuest::questCompleted(name))
    {
        std::cerr << "Quest " << name << " << is completed. You can't activate a completed quest, use reset first."
                  << std::endl;
    }
}

void MetaQuest::forceCompleteQuest(std::string name)
{
    if(MetaQuest::questActive(name)) {
        MetaQuest::activeQuests[name]->forceComplete();
        QuestMenu::getSingletonPtr()->removeQuestIcon(name);
    } else if(MetaQuest::questInactive(name)) {
        MetaQuest::inactiveQuests[name]->forceComplete();
    }

    MetaQuest::completeQuest(name);
}

void MetaQuest::silentCompleteQuest(std::string name)
{
    MetaQuest::completeQuest(name);
}

bool MetaQuest::questActive(std::string name)
{
    return MetaQuest::mapContainsQuest(MetaQuest::activeQuests, name);
}

bool MetaQuest::questInactive(std::string name)
{
    return MetaQuest::mapContainsQuest(MetaQuest::inactiveQuests, name);
}

bool MetaQuest::questCompleted(std::string name)
{
    return MetaQuest::mapContainsQuest(MetaQuest::completedQuests, name);
}

void MetaQuest::completeQuest(std::string name)
{
    if(MetaQuest::questActive(name))
    {
        MetaQuest::completedQuests[name] = MetaQuest::activeQuests[name];
        MetaQuest::activeQuests.erase(name);
        QuestMenu::getSingletonPtr()->removeQuestIcon(name);
    }

    else if(MetaQuest::questInactive(name))
    {
        MetaQuest::completedQuests[name] = MetaQuest::inactiveQuests[name];
        MetaQuest::inactiveQuests.erase(name);
    }
}

void MetaQuest::resetQuest(std::string name)
{
    if(MetaQuest::questActive(name))
    {
        MetaQuest::inactiveQuests[name] = MetaQuest::activeQuests[name];
        MetaQuest::inactiveQuests[name]->reset();
        MetaQuest::activeQuests.erase(name);
        script::interpret("print('Reset Active Quest')");
        QuestMenu::getSingletonPtr()->removeQuestIcon(name);
    }

    else if(MetaQuest::questCompleted(name))
    {
        MetaQuest::inactiveQuests[name] = MetaQuest::completedQuests[name];
        MetaQuest::inactiveQuests[name]->reset();
        MetaQuest::completedQuests.erase(name);
        script::interpret("print('Reset Completed Quest')");
    }

    else if(MetaQuest::questInactive(name))
    {
        MetaQuest::inactiveQuests[name]->reset();
        script::interpret("print('Reset Inactive Quest')");
    }
}

void MetaQuest::reset()
{
    quest_map_t::iterator activeIter = MetaQuest::activeQuests.begin();
    while(activeIter != MetaQuest::activeQuests.end())
    {
        QuestMenu::getSingletonPtr()->removeQuestIcon(activeIter->second->getName());
        delete activeIter->second;
        ++activeIter;
    }

    MetaQuest::activeQuests.clear();

    quest_map_t::iterator inactiveIter = MetaQuest::inactiveQuests.begin();
    while(inactiveIter != MetaQuest::inactiveQuests.end())
    {
        delete inactiveIter->second;
        ++inactiveIter;
    }

    MetaQuest::inactiveQuests.clear();

    quest_map_t::iterator completedIter = MetaQuest::completedQuests.begin();
    while(completedIter != MetaQuest::completedQuests.end())
    {
        delete completedIter->second;
        ++completedIter;
    }

    MetaQuest::completedQuests.clear();
}

const quest_map_t& MetaQuest::getActiveQuests()
{
    return MetaQuest::activeQuests;
}

const quest_map_t& MetaQuest::getInactiveQuests()
{
    return MetaQuest::inactiveQuests;
}

const quest_map_t& MetaQuest::getCompletedQuests()
{
    return MetaQuest::completedQuests;
}

bool MetaQuest::mapContainsQuest(quest_map_t& map, std::string questName)
{
    quest_map_t::iterator find = map.find(questName);
    if(find != map.end())
        return true;
    else
        return false;
}

/*
// Singleton stuff

#if (OGRE_VERSION < ((1 << 16) | (8 << 8)))
template<> MetaQuest* Ogre::Singleton<MetaQuest>::ms_Singleton = 0;
MetaQuest* MetaQuest::getSingletonPtr(void)
{
    return ms_Singleton;
}
MetaQuest& MetaQuest::getSingleton(void)
{
    assert( ms_Singleton );  return ( *ms_Singleton );
}
#else
template<> MetaQuest* Ogre::Singleton<MetaQuest>::msSingleton = 0;
MetaQuest* MetaQuest::getSingletonPtr(void)
{
    return msSingleton;
}
MetaQuest& MetaQuest::getSingleton(void)
{
    assert( msSingleton );  return ( *msSingleton );
}
#endif
*/

// CREATE THE ACTUAL OBJECT INSTANCE
//MetaQuest metaQuest;

} // gameplay namespace
