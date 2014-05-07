--Create Items Example Code

--[[

Item functions:

addItem(itemName, iconName, description)
removeItem(itemName)
combineItems(itemName1, itemName2)
addUseItemAction(itemName, targetObjectType, action) -- action is a string!!!
removeUseItemAction(itemName, targetObjectType)
useAction(itemName, targetObjectType)

]]

--Create an item

--addItem("Flashlight", "Flashlight.png", "Pretend you are Dora, the Explorer")
--addUseItemAction("Flashlight", "default", "print('Let there be light.')")

--addItem("SonicPathfinder", "SonicPathfinder.png", "Who are you gonna call?")
--addUseItemAction("SonicPathfinder", "default", "print('Hello, is this the ghostbusters?')")

-- Create a namespace for functions that items call
itemFunctions = {}

itemFunctions.addNewQuest = function()
	addNewQuest('ItemQuest1', false, print('Adding ItemQuest1'), 'This is ItemQuest1.')
	activateQuest('ItemQuest1')
end

addItem("StrangeRock", "StrangeRock.png", "Diego is dora's cousin.")
addUseItemAction("StrangeRock", "default", itemFunctions.addNewQuest) --call our function from the itemFunctions namespace