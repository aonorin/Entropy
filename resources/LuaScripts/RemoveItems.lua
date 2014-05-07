--Remove Items Example Code

--[[

Item functions:

addItem(itemName, iconName, description)
removeItem(itemName)
combineItems(itemName1, itemName2)
addUseItemAction(itemName, targetObjectType, action) -- action is a string!!!
removeUseItemAction(itemName, targetObjectType)
useAction(itemName, targetObjectType)

]]

--remove a use action

--removeUseItemAction("Flashlight", "default")
--removeItem("Flashlight")

--removeUseItemAction("SonicPathfinder", "default")
--removeItem("SonicPathfinder")

removeUseItemAction("StrangeRock", "default")
removeItem("StrangeRock")

--remove an item