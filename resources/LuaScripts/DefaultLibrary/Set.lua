--Set
--Class representing a collection of unique items

require 'class'

Set = class(function(s)
	s.internalTable = {}
	end
)

function Set:insertValue(value)
	self.internalTable[value] = true
end

function Set:removeValue(value)
	self.internalTable[value] = nil
end

function Set:contains(value)
	return self.internalTable[value] ~= nil
end

function Set:toTable()
	return self.internalTable
end

function Set:clear()
	s.internalTable = nil
	s.internalTable = {}
end