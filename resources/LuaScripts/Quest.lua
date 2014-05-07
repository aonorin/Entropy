--Quest Class
require 'class'

-- single item to accompolish in a quest 
QuestItem = class(function(q, name, action)
	q.name = name
	q.completed = false
	q.completionAction = action
	end
)

function QuestItem:setCompleted(completed)
	local trigger = false
	
	if completed == true and self.completed == false then 
		trigger = true
	end
	
	self.completed = completed
	if trigger then 
		if self.completionAction ~= nil then
			self.completionAction()
		end
	end
end

function QuestItem:__tostring()
	return self.name..' completed: '..tostring(self.completed)
end

-- a collection of quest items to complete with a completion action
Quest = class(function(q, name, action)
	q.name = name
	q.completed = false
	q.completionAction = action
	q.questItems = {}
	q.numQuestItems = 0
	end
)
		
function Quest:__tostring()
	local printString = self.name..' completed: '..tostring(self.completed)
	for k,v in pairs(self.questItems) do
		printString = printString..'\n	'..v:__tostring()
	end
	return printString
end

function Quest:name()
	return self.name
end

function Quest:isCompleted()
	return self.completed
end

function Quest:addQuestItem(name, action)
	self.questItems[name] = QuestItem(name, action)
	self.numQuestItems = self.numQuestItems + 1
	self:update()
end

function Quest:removeQuestItem(name)
	self.questItems[name] = nil
	self.numQuestItems = self.numQuestItems - 1
	self:update()
end

function Quest:setQuestItemCompleted(name, completed)
	self.questItems[name]:setCompleted(completed)
	self:update()
end

function Quest:numberOfQuestItems()
	return self.numQuestItems
end

-- Evaluates the quest items and updates the completion status, returning the value
function Quest:update()
	
	if self:numberOfQuestItems() == 0 then 	-- Make sure we're not empty
		self.completed = false
		return self.completed
	end
	
	local completionCheck = true -- default to true then set to false and exit if any quest items aren't finished
	
	for k,v in pairs(self.questItems) do
		if v.completed == false then 
			self.completed = false 
			return self.completed
		end
	end
	
	if completionCheck == true and self.completed == false then  -- if true call the completion action, if any
		if self.completionAction ~= nil then
			self.completionAction()
		end
	end
	
	self.completed = completionCheck
	return self.completed
end

function Quest:forceComplete()
	
	for k,v in pairs(self.questItems) do
		v:setCompleted(true)
	end
	
	self:update()
end