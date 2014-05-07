-- Being Quests Example Code

function quest1CompleteAction()
	print("Quest1 COMPLETED!!!!!!!!")
end

function quest1Item1CompleteAction()
	print("Quest1 Item1 completed.")
end

function quest1Item2CompleteAction()
	print("Quest1 Item2 completed.")
end

function quest1Item3CompleteAction()
	print("Quest1 Item3 completed.")
end

print('creating Quest1')
addNewQuest("Quest1", false, quest1CompleteAction, "This is Quest1.")
addQuestItem("Quest1", "QuestItem1", quest1Item1CompleteAction, "This is Quest1 item1.")
addQuestItem("Quest1", "QuestItem2", quest1Item2CompleteAction, "This is Quest1 item2.")
addQuestItem("Quest1", "QuestItem3", quest1Item3CompleteAction, "This is Quest1 item3.")

function quest2CompleteAction()
	print("Quest2 COMPLETED!!!!!!!!")
end

function quest2Item1CompleteAction()
	print("Quest2 Item1 completed.")
end

function quest2Item2CompleteAction()
	print("Quest2 Item2 completed.")
end

function quest2Item3CompleteAction()
	print("Quest2 Item3 completed.")
end

print('creating Quest2')
addNewQuest("Quest2", false, quest2CompleteAction, "This is Quest2.")
addQuestItem("Quest2", "QuestItem1", quest2Item1CompleteAction, "This is Quest2 item1.")
addQuestItem("Quest2", "QuestItem2", quest2Item2CompleteAction, "This is Quest2 item2.")
addQuestItem("Quest2", "QuestItem3", quest2Item3CompleteAction, "This is Quest2 item3.")


print('Quest1 is inactive, and this should print true')
print(questInactive('Quest1'))

print('Quest2 is inactive, and this should print true')
print(questInactive('Quest2'))


print('Activating the Quests')
activateQuest('Quest1')
activateQuest('Quest2')


print('Quest1 is active, and this should print true')
print(questActive('Quest1'))

print('Quest2 is active, and this should print true')
print(questActive('Quest2'))