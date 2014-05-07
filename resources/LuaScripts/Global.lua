--Global Entropy Namespace

require 'EventProcessor'
require 'Actor'
require 'Quest'
require 'WorldItem'
require 'ModelTemplate'
require 'Physics'

--Global Variables, DO NOT CHANGE THESE DIRECTLY, ONLY REFERENCE IF NECESSARY------------------------
glEvents = EventProcessor()
glObjectTable = {}
glInventoryItemTable = {}
glQuestTable = {}
glModelTable = {}
glUniqueObjectCounter = 0
glUniqueInventoryItemCounter = 0
glPhysics = Physics()
-----------------------------------------------------------------------------------------------------

function addModel(model) -- Supply the actual model instance (not an actor)
	glModelTable[model:name()] = model
end

function removeModel(model) -- Supply the actual model instance (not an actor)
	glModelTable[model:name()] = nil
end

function spawnActor(modelName, x, y, z) -- Creates a new object from a model and position, returning the unique ID
	local id = glUniqueObjectCounter
	local position = {x, y, z}
	glObjectTable[id] = Actor(id, modelName, position)
	glUniqueObjectCounter = glUniqueObjectCounter + 1
	return id
end

function spawnWorldItem(modelName, x, y, z)
	local id = glUniqueObjectCounter
	local position = {x, y, z}
	glWorldItemTable[id] = WorldItem(id, modelName, position)
	glUniqueObjectCounter = glUniqueObjectCounter + 1
	return id
end

function destroy(objectID) 
	glObjectTable[objectID]:destroy()
	glObjectTable[objectID] = nil
end

function updateObject()
	glEvents:update(glObjectTable)
end

function animateObjects()
	glEvents:animate(glObjectTable)
end

function registerObjectUpdate(objectID)
	glEvents:registerUpdate(objectID)
end

function unregisterObjectUpdate(objectID)
	glEvents:unregisterUpdate(objectID)
end

function registerObjectAnimation(objectID)
	glEvents:registerAnimation(objectID)
end

function unregisterObjectAnimation(objectID)
	glEvents:unregisterAnimation(objectID)
end

function addInventoryItem(modelName) -- Creates a new InventoryItem from a model, returning the unique ID
	local id = glUniqueInventoryItemCounter
	glInventoryItemTable[modelName] = InventoryItem(id, modelName)
	glUniqueInventoryItemCounter = glUniqueInventoryItemCounter + 1
	return id
end

function removeInventoryItem(objectID) -- Remove an Inventory item, takes the ObjectID, NOT model name
	glInventoryItemTabl[objectID] = nil
end

function reqisterQuest(quest)
	glQuestTable[quest:name()] = quest
end

function unregisterQuest(quest)
	glQuestTable[quest:name()] = nil
end

function forceCompleteQuest(name)
	glQuestTable[name]:forceComplete()
	glQuestTable[name] = nil
end

function globalReset()
	
	glEvents:reset()
	
	local objects = glObjectTable
	for k,v in pairs(objects) do
		destroy(v:id())
	end
	
	local inventoryItems = glInventoryItemTable
	for k,v in pairs(inventoryItems) do
		glInventoryItemTable[v:id()] = nil
	end
	
	local quests = glQuestTable
	for k,v in pairs(quests) do
		glQuestTable[v:name()] = nil
	end
	
	local models = glModelTable
	for k,v in pairs(models) do
		glModelTable[v:name()] = nil
	end
	
	glUniqueObjectCounter = 0
	glUniqueInventoryItemCounter = 0
	
end

function setGravity(x, y, z) -- Set the gravity with x, y, and z components
	local gravity = {x, y, z}
	glPhysics:setGravity(gravity)
end

function gravity()
	return glPhysics:gravity()
end

function applyForce(objectID, x, y, z) -- Apply force to an Actor with x, y, and z components
	local forceVector = {x, y, z}
	glObjectTable[objectID]:applyForce(forceVector)
end

function collideObject(objectID1, objectID2) --Call the collision actions between two objects, referenced by ID
	glObjectTable[objectID1]:collideWithObject(glObjectTable[objectID2]:modelName())
	glObjectTable[objectID2]:collideWithObject(glObjectTable[objectID1]:modelName())
end