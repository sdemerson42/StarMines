function Treasure_calls(bc)
	local call = bc:getCall()
	if call.label == "collision" and call.tag == "RedGuy" then
		bc:playSound("Coin", 40, true, false)
		bc:sendToTag("Logic", "treasure", "")
		bc:despawn("")
	end
end

function Treasure_pulse(bc)
	local scale = bc:getRegFloat("scale")
	local scaleDir = bc:getRegInt("scaleDir")
	
	if scaleDir == 0 then
		scale = scale + 0.02
	else
		scale = scale - 0.02
	end

	bc:scale(scale, scale)

	if scale >= 1.5 then
		bc:setRegFloat("scaleDir", 1)
	end
	if scale <= 1.0 then
		bc:setRegFloat("scaleDir", 0)
	end
	bc:setRegFloat("scale", scale)
end

function Treasure(bc)
	Treasure_pulse(bc)
	Treasure_calls(bc)	
end