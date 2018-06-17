function Logic_spawnBots(bc)
	bc:incRegInt(0)
	if bc:getRegInt(0) == 40 then
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
		bc:setRegInt(0, 0)
		bc:spawn("Bot", x, y, speed)
	end
end

function Logic_setScore(bc)
	local txt = "Score: "
	txt = txt .. bc:getRegInt(1)
	bc:setText(txt)
end

function Logic_calls(bc)
	local call = bc:getCall()
	if call.label ~= "nil" then
		if call.label == "incScore" then bc:incRegInt(1) end
		call = bc:getCall()
	end

	if bc:getRegInt(1) == 20 then
		bc:incRegInt(5)
		bc:sendToTag("RedGuy", "win", "")
		bc:setPosition(290, 280)
		bc:newScene("Alt")
		bc:setText("YOU R A WINNER!")
	end
end

function Logic(bc)
	if bc:getRegInt(5) == 1 then return end
	Logic_calls(bc)
	if bc:getRegInt(5) == 1 then return end
	Logic_setScore(bc)
	Logic_spawnBots(bc)	
end