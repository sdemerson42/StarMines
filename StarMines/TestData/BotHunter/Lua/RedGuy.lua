-- Registers: 0 = facing, 1 = shoot cooldown counter
-- 2 = FlameSound
-- 3 = Dead / win

function RedGuy_move(bc, x, y)
	bc:setSpeed(6)
	bc:setDir(x,y)
	
	local facing = bc:getRegInt(0)

	if x == 0 and y == 0 then
		if facing == 1 then bc:playAnim("idleR")
		else bc:playAnim("idleL") end
	end
	
	if x < 0 or facing == 0 then bc:playAnim("left") end
	if x > 0 or facing == 1 then bc:playAnim("right") end
end

function RedGuy_fire(bc, u, v)
	bc:setDir(0,0)
	if bc:getRegInt(0) == 1 then bc:playAnim("idleR")
	else bc:playAnim("idleL") end
	
	if bc:getRegInt(1) == 0 then
		bc:setRegInt(1, 5)
		dirstr = ""
		dirstr = u .. "," .. v
	
		local pos = bc:position()
		bc:spawn("Fire", pos.x, pos.y, dirstr)
	end

	if bc:getRegInt(2) == 0 then
		bc:setRegInt(2,1)
		bc:playSound("Flame", 40, true, true)
	end
end

function RedGuy_input(bc)
	local inp = bc:input()
	local x = inp.x
	local y = inp.y
	local u = inp.u
	local v = inp.v
	
	-- If firing flamethrower, movement phase is canceled
	
	if u ~= 0 or v ~= 0 then 
		RedGuy_fire(bc, u, v)
	else
		bc:stopSound("Flame")
		bc:setRegInt(2,0)
		if x < 0 then bc:setRegInt(0, 0) end
		if x > 0 then bc:setRegInt(0, 1) end
		RedGuy_move(bc, x, y)
	end
end

function RedGuy_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and call.tag == "Bot" then
			bc:stopSound("Flame")
			bc:playSound("GameOver", 40, true, false)
			local pos = bc:position()
			bc:spawn("Grave", pos.x, pos.y, "")
			bc:despawn("")
			bc:incRegInt(3)
			break
		end

		if call.label == "win" then
			bc:stopSound("Flame")
			bc:incRegInt(3)
			bc:deactivate("")
			return
		end

		call = bc:getCall()
	end
end

function RedGuy(bc)
	if bc:getRegInt(3) ~= 0 then return end
	RedGuy_calls(bc)
	if bc:getRegInt(3) ~= 0 then return end
	RedGuy_input(bc)
	if bc:getRegInt(1) > 0 then bc:decRegInt(1) end	
end