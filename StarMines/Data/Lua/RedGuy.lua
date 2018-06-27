function RedGuy_move(bc, x, y)
	bc:setSpeed(6)
	bc:setDir(x,y)

	local facing = bc:getRegInt("face")

	if x == 0 and y == 0 then
		if facing == 1 then bc:playAnim("idleR")
		else bc:playAnim("idleL") end
	end
	
	if x < 0 or facing == 0 then bc:playAnim("left") end
	if x > 0 or facing == 1 then bc:playAnim("right") end
end

function RedGuy_fire(bc, u, v)
	bc:setDir(0,0)
	if bc:getRegInt("face") == 1 then bc:playAnim("idleR")
	else bc:playAnim("idleL") end
	
	if bc:getRegInt("cool") == 0 then
		bc:setRegInt("cool", 5)
		dirstr = ""
		dirstr = u .. "," .. v
	
		local pos = bc:position()
		bc:spawn("Fire", pos.x, pos.y, dirstr, "default")
	end

	if bc:getRegInt("sound") == 0 then
		bc:setRegInt("sound",1)
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
		bc:setRegInt("sound",0)
		if x < 0 then bc:setRegInt("face", 0) end
		if x > 0 then bc:setRegInt("face", 1) end
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
			bc:spawn("Grave", pos.x, pos.y, "", "default")
			bc:despawn("")
			bc:incRegInt("inactive")
			bc:sendToTag("Logic", "gameOver", "")
			break
		end

		call = bc:getCall()
	end
end

function RedGuy(bc)
	if bc:getRegInt("inactive") ~= 0 then return end
	RedGuy_calls(bc)
	if bc:getRegInt("inactive") ~= 0 then return end
	RedGuy_input(bc)
	if bc:getRegInt("cool") > 0 then bc:decRegInt("cool") end
end