function Blue(bc)
	local pos = bc:position()
	local x = pos.x
	local y = pos.y
	x = x + math.random(-1,1)
	y = y + math.random(-1,1)
	bc:setPosition(x,y)
end