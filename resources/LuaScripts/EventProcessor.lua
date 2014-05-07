--EventProcessor class
--processes events and updates objects that are registered

require 'class'
require 'Set'

EventProcessor = class(function(e)
	e.updateSet = Set()
	e.animationSet = Set()
	end
)

function EventProcessor:update(objectTable)
	for k,v in pairs(self.updateSet:toTable()) do
		objectTable[v]:update()
	end
end

function EventProcessor:animate(objectTable)
	for k,v in pairs(self.animationSet:toTable()) do
		objectTable[v]:animate()
	end
end

function EventProcessor:reset()	
	self.updateSet:clear()
	self.animationSet:clear()
end

function EventProcessor:registerUpdate(objectID)
	self.updateSet:insertValue(objectID)
end

function EventProcessor:registerAnimation(objectID)
	self.animationSet:insertValue(objectID)
end

function EventProcessor:unregisterUpdate(objectID)
	self.updateSet:removeValue(objectID)
end

function EventProcessor:unregisterAnimation(objectID)
	self.updateSet:removeValue(objectID)
end