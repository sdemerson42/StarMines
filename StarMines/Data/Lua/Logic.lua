function Logic_spawnBots(bc)
	bc:incRegInt("botCounter")
	if bc:getRegInt("botCounter") == 40 then
		local x,y = 0,0
		local r = math.random(1,2)
		if r == 1 then
			r = math.random(1,2)
			if (r == 1) then x = 0
			else x = 770 end
			y = math.random(0, 570)
		else
			r = math.random(1,2)
			if (r == 1) then y = 0
			else y = 570 end
			x = math.random(0, 770)
		end
		
		local speed = math.random(1, 6)
		bc:setRegInt("botCounter", 0)
		bc:spawn("Bot", x, y, speed, "default")
	end
end

function Logic_setScore(bc)
	local txt = "Score: "
	txt = txt .. bc:getRegInt("score")
	bc:setText(txt)
end

function Logic_calls(bc)
	local call = bc:getCall()
	if call.label ~= "nil" then
		if call.label == "incScore" then 
			local scalar = stoary(call.sdata)
			scalar[1] = scalar[1] * 5
			bc:setRegInt("score", bc:getRegInt("score") + scalar[1])
		end

		if call.label == "treasure" then bc:setRegInt("score", bc:getRegInt("score") + 50) end
		if call.label == "gameOver" then bc:setRegInt("deathCount", 1) end
		call = bc:getCall()
	end

	if bc:getRegInt("score") >= 500 then
		bc:incRegInt("inactive")
		bc:newScene("Alt")
	end
end

function Logic_spawnTreasure(bc)
	local r = math.random(1,100)
	if r == 100 then
		local x = math.random(100, 670)
		local y = math.random(100, 470)
		bc:spawn("Treasure", x, y, "", "default")
	end
end

function Logic(bc)
	if bc:getRegInt("deathCount") > 0 then
		bc:incRegInt("deathCount")
		if bc:getRegInt("deathCount") == 120 then
			bc:setRegInt("deathCount", 0)
			bc:newScene("Main")
		end
	end

	if bc:getRegInt("inactive") == 1 then return end
	Logic_calls(bc)
	if bc:getRegInt("inactive") == 1 then return end
	Logic_setScore(bc)
	Logic_spawnBots(bc)
	Logic_spawnTreasure(bc)
end