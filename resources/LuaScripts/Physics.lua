--Physics
--Controls the global physics settings

require 'class'

Physics = class(function(p)
	p.gravity = {0, -180, 0}
	end
)

function Physics:reset()
	self.gravity = {0, -180, 0}
end

function Physics:setGravity(gravityVector)
	self.gravity = gravityVector
end

function Physics:gravity()
	return self.gravity
end