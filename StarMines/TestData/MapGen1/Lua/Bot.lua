function Bot_attack(bc)
	bc:setRegInt("counter",60)
	bc:setTargetByTag("RedGuy", "first")
	local tpos = bc:targetPosition()
	local pos = bc:position()
	local x = tpos.x - pos.x
	local y = tpos.y - pos.y
	bc:setDir(x,y)
	bc:playAnim("active")
end

function Bot_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and call.tag == "Fire" then
			bc:playSound("Die", 40, false, false)
			local speed = bc:speed()
			bc:sendToTag("Logic", "incScore", speed)
			
			-- Plasma
			
			local pos = bc:position()
			bc:spawn("Plasma", pos.x, pos.y, "1,0") 
			bc:spawn("Plasma", pos.x, pos.y, "0,1")
			bc:spawn("Plasma", pos.x, pos.y, "-1,0")
			bc:spawn("Plasma", pos.x, pos.y, "0,-1")
			bc:spawn("Plasma", pos.x, pos.y, "1,1")
			bc:spawn("Plasma", pos.x, pos.y, "-1,-1")
			bc:spawn("Plasma", pos.x, pos.y, "-1,1")
			bc:spawn("Plasma", pos.x, pos.y, "1,-1")
			
			bc:despawn("")
		end

		if call.label == "init" then
			local ary = stoary(call.sdata)
			bc:setSpeed(ary[1])
		end
	
		call = bc:getCall()
	end
end

function Bot(bc)
	Bot_calls(bc)
	if bc:getRegInt("counter") == 0 then Bot_attack(bc) end
	bc:decRegInt("counter")
end