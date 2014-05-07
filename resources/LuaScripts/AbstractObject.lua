--AbstractObject class
-- Actor and WorldItem inherits from Object. It represents an in game object, NOT the mesh/materials that are referenced by it

require 'class'

-- objectID is the unique identifier for this object, the objectName is the object 'template' used, position is world position
AbstractObject = class(function(a, objectID, objectModelName, objectPosition)
	a.updateActionTable = {}
	a.animateActionTable = {}
	a.destroyActionTable = {}
	a.valueTable = {id=objectID, modelName=objectModelName, position=objectPosition}
	a.collisionActionTable = {} -- A Dictionary of object names and functions
	end
)

function AbstractObject:addUpdateAction(name, action)
	self.updateActionTable[name] = action
end

function AbstractObject:removeUpdateAction(name)
	self.updateActionTable[name] = nil
end

function AbstractObject:update()
	for k,v in pairs(self.updateActionTable) do
		v()
	end
end

function AbstractObject:addAnimationAction(name, action)
	self.animateActionTable[name] = action
end

function AbstractObject:removeAnimationAction(name)
	self.animateActionTable[name] = nil
end

function AbstractObject:animate()
	for k,v in pairs(self.animateActionTable) do
		v()
	end
end

function AbstractObject:getValue(valueName)
	return self.valueTable[valueName]
end

--set value will add the value to the table if it does not exist
function AbstractObject:setValue(valueName, value)
	self.valueTable[valueName] = value
end

function AbstractObject:id()
	return self.valueTable["id"]
end

function AbstractObject:modelName()
	return self.valueTable["modelName"]
end

function AbstractObject:setModelName(modelName)
	self.valueTable["modelName"] = modelName
end

function AbstractObject:position()
	return self.valueTable["position"]
end

function AbstractObject:setPosition(position)
	self.valueTable["position"] = position
end

function AbstractObject:addDestroyAction(name, action)
	self.destroyActionTable[name] = action
end

function AbstractObject:removeDestroyAction(name)
	self.destroyActionTable[name] = nil
end

function AbstractObject:destroy()
	for k,v in pairs(self.destroyActionTable) do
		v()
	end
end

function AbstractObject:addCollisionAction(modelName, action) --reference by Model Name NOT objectID
	self.collisionActionTable[modelName] = action
end

function AbstractObject:removeCollisionAction(modelName)
	self.collisionActionTable[modelName] = nil
end

function AbstractObject:collideWithObject(modelName)
	if self.collsionActionTable[modelName] ~= nil then
		self.collisionActionTable[modelName]()
	end
end
	
function AbstractObject:applyForce(forceVector) --Apply force with 3D vector
	--Do C++ physics call here.
end
