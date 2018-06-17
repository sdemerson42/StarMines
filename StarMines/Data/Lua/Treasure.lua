function Treasure_calls(bc)
	local call = bc:getCall()
	if call.label == "collision" and call.tag == "RedGuy" then
		bc:playSound("Coin", 40, true, false)
		bc:sendToTag("Logic", "treasure", "")
		bc:despawn("")
	end
end

function Treasure(bc)
	Treasure_calls(bc)	
end