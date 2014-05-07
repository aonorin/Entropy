--Item class
--inherits from AbstractObject

require 'AbstractObject'

WorldItem = class(AbstractObject, function(w, objectID, objectModelName, objectPosition)
	AbstractObject.init(w, objectID, objectModelName, objectPosition)
	w.description = "No description available for this object."
	local default = function() print("Nothing happens.") end
	w.useActionTable = {defaultAction=default} --Dictionary of object names and functions to take on them
	end
)

function WorldItem:setDescription(description)
	self.description = description
end

function WorldItem:description()
	return self.description
end

function WorldItem:addAction(name, action)
	self.useActionTable[name] = action
end

function WorldItem:removeAction(name, action)
	self.useActionTable[name] = nil
end

function WorldItem:useAction(name)
	self.useActionTable[name]()
end