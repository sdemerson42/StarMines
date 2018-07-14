function Fire_init(bc)
	bc:setRegInt("init", 1)
	local call = bc:getCall()
	local dir = stoary(call.sdata)
	local r = math.random(-30,30)
	dir[1] = dir[1] + r
	r = math.random(-30,30)
	dir[2] = dir[2] + r
	bc:setDir(dir[1], dir[2])
	bc:setSpeed(8)
	bc:rotate(math.random(-20, 20))
end

function Fire(bc)
	if bc:getRegInt("init") == 0 then Fire_init(bc) end

	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and bc:callerHasTag("Barrier") then
			bc:deactivate()
			bc:despawn("")
		end 
		call = bc:getCall()
	end
	
	local counter = bc:getRegInt("counter")
	if counter == 30 then bc:despawn("")
	else bc:setRegInt("counter", counter + 1) end
end