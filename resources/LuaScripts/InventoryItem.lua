--InventoryItem class
--defines items in the players inventory and their actions

require 'class'

InventoryItem = class(function(i, objectID, objectModelName)
	i.id = objectID
	i.modelName = objectModelName
	i.description = "No description available for this item."
	i.useActionTable = {} --Dictionary of object names and functions to take on them
	end
)

function InventoryItem:modelName()
	return self.modelName
end

function InventoryItem:setModelName(modelName)
	self.modelName = modelName
end

function InventoryItem:id()
	return self.id
end

function InventoryItem:setID(id)
	self.id = id
end

function InventoryItem:description()
	return self.description
end

function InventoryItem:setDescription(description)
	self.description = description
end

function InventoryItem:addUseAction(objectModelName, action) -- Supply the Object Model Name, not the id or the object itself
	self.useActionTable[objectModelName] = action
end

function InventoryItem:removeUseAction(objectModelName) -- Supply the Object Model Name, not the id or the object itself
	self.useActionTable[objectModelName] = nil
end

function InventoryItem:useOn(objectModelName) -- Supply the Object Model Name, not the id or the object itself
	self.useActionTable[objectModelName]()
end