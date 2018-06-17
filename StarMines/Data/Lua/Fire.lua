function Fire_init(bc)
	bc:setRegInt(0, 1)
	local call = bc:getCall()
	local dir = stoary(call.sdata)
	local r = math.random(-30,30)
	dir[1] = dir[1] + r
	r = math.random(-30,30)
	dir[2] = dir[2] + r
	bc:setDir(dir[1], dir[2])
	bc:setSpeed(8)
end

function Fire(bc)
	if bc:getRegInt(0) == 0 then Fire_init(bc) end
	
	local counter = bc:getRegInt(1)
	if counter == 30 then bc:despawn("")
	else bc:setRegInt(1, counter + 1) end
end