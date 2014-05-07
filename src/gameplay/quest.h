#ifndef QUEST_H
#define QUEST_H

// System includes
#include <string>
#ifndef WIN32
#include <tr1/unordered_map>
#else
#include <unordered_map>
#endif

#include <vector>

// Boost includes
#include "boost/function.hpp"
#include "boost/variant.hpp"

// Ogre includes
#include "OGRE/OgreSingleton.h"
//#include "OGRE/OgreTextAreaOverlayElement.h"

// Entropy includes
#include "src/Script/scripttypes.h"

namespace gameplay {

/////////////////////////////////
// Quest Completed Action types
/////////////////////////////////

// Used in Quest completion actions so that arguments passed can be characters or doubles
typedef boost::variant<double, std::string, bool> quest_argument_t;
typedef std::vector<quest_argument_t> quest_args_t;

// The typedef for Quest Completed Actions, must take a vector of QuestActionArgument's
typedef boost::function<void (quest_args_t&)> quest_action_t;

// default action, does nothing
void defaultQuestAction(quest_args_t& arguments);
extern quest_args_t emptyArgs; // Default empty arguments

// QuestActions struct for holding both the quest action function and the arguments to pass it
struct QuestAction
{
    quest_args_t args;
    quest_action_t action;
    QuestAction(quest_args_t& args = emptyArgs,
                quest_action_t action = defaultQuestAction)
        : args(args), action(action) {}

    void operator()()
    {
        action(args);
    }

    void doAction()
    {
        action(args);
    }
};

/////////////
// QuestItem
/////////////

class QuestItem
{
public:

    QuestItem(std::string name, script::lua_functor_t completedAction, std::string description = "");
    ~QuestItem();
    void setCompleted(bool completed);
    bool getCompleted();
    std::string getName();
    void setName(std::string name);
    std::string getDescription();
    void setDescription(std::string description);

private:
    bool completed;
    script::lua_functor_t completedAction;
    std::string name;
    std::string description;
};

/////////
// Quest
/////////

class Quest
{
public:
    Quest(std::string name, script::lua_functor_t completedAction, std::string description = "");
    ~Quest();

    void addQuestItem(std::string itemName, script::lua_functor_t completedAction, std::string description);
    void removeQuestItem(std::string itemName);
    void forceComplete();
    void reset(); // Resets this quest, setting its items to incomplete status
    int numberOfQuestItems();
    void setQuestItemCompleted(std::string itemName, bool completed);
    bool getQuestItemCompleted(std::string itemName);
    bool getCompleted();
    std::string getName();
    void setName(std::string name);
    std::string getDescription();
    void setDescription(std::string description);
    std::string getQuestItemDescription(std::string itemName);
    void setQuestItemDescription(std::string itemName, std::string description);
    bool containsQuestItem(std::string itemName);
    bool update();
    std::tr1::unordered_map<std::string, QuestItem*>* getQuestItems();

private:

    std::tr1::unordered_map<std::string, QuestItem*> questItems;
    bool completed;
    script::lua_functor_t completedAction;
    std::string name;
    std::string description;
};

/////////////
// MetaQuest
/////////////

// MetaQuest is the central repository for quests that are currently inactive, active, or completed
// All access to them and their QuestItems is through the MetaQuest singleton

typedef std::tr1::unordered_map<std::string, Quest*> quest_map_t;

class MetaQuest
//        : public Ogre::Singleton<MetaQuest>
{
public:

    MetaQuest();
    ~MetaQuest();

    static void addQuest(Quest* quest, bool setActive = false); // Add a new quest to the inactive quest map
    static void addNewQuest(std::string questName, bool setActive, script::lua_functor_t completedAction,
                            std::string description = "");
    static void addQuestItem(std::string questName, std::string questItemName, script::lua_functor_t completedAction,
                             std::string description = "");
    static void setQuestItemCompleted(std::string questName, std::string questItemName, bool completed);
    static std::string getQuestDescription(std::string questName);
    static std::string getQuestItemDescription(std::string questName, std::string questItemName);
    static void setQuestDescription(std::string questName, std::string description);
    static void setQuestItemDescription(std::string questName, std::string questItemName, std::string description);
    static void activateQuest(std::string name); // Activate a quest
    static void forceCompleteQuest(std::string name); // Complete a quest, calling all its completionActions
    static void silentCompleteQuest(std::string name); // Move a quest to the completedMap without calling its actions
    static bool questActive(std::string name);
    static bool questInactive(std::string name);
    static bool questCompleted(std::string name);
    static void resetQuest(std::string name); // Resets a single quest, moving it to inactive and setting to incomplete
    static void reset(); // Clears everything out, DOES NOT CALL QUEST COMPLETED ACTIONS
    static const quest_map_t& getActiveQuests();
    static const quest_map_t& getInactiveQuests();
    static const quest_map_t& getCompletedQuests();

//    static MetaQuest& getSingleton(void);
//    static MetaQuest* getSingletonPtr(void);

//    friend class Quest; // So Quest can move itself to completed when its done
    static void completeQuest(std::string name); // complete a quest, moving it from the active to completed maps
    static bool mapContainsQuest(quest_map_t& map, std::string questName);

    static quest_map_t activeQuests;
    static quest_map_t inactiveQuests;
    static quest_map_t completedQuests;
};

//extern MetaQuest metaQuest;

} // gameplay namespace

#endif // QUEST_H
