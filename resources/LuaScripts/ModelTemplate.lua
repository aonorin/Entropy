--ModelTemplate
--Defines a collection of resources used by Objects or other classes/scripts
--Commonly this would include all the meshes, textures, materials, etc... associated with a given 'Model'

require 'class'

ModelTemplate = class(function(r, name)
	r.name = name
	r.valueTable = {}
	end
)

function ModelTemplate:addResource(name, resource)
	self.valueTable[name] = resource
end

function ModelTemplate:removeResource(name)
	self.valueTable[name] = nil
end