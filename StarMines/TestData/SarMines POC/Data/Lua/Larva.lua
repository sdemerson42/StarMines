function Larva_navigate(bc)
	local pos = bc:position()
	local tPos = bc:targetPosition()
	bc:setDir(tPos.x - pos.x, tPos.y - pos.y)
	bc:incRegInt("corrections")
	if bc:getRegInt("corrections") > 5 then
		bc:setTargetByTag("ShipIcon", "first")
	end
end

function Larva_start(bc)
	bc:incRegInt("start")
	if math.random(1,2) == 1  or g_cart == 0 then
		bc:setTargetByTag("ShipIcon", "first")
	else
		bc:setTargetByTag("Cart", "near")
	end
	bc:setSpeed(1 + .1 * (g_turn - 1))
	Larva_navigate(bc)
end

function Larva_calls(bc)
	local call = bc:getCall()
	while call.label ~="nil" do
		if call.label == "collision" and call.tag == "SentryShot" then
			bc:playSound("Die", 40, true, false)
			bc:despawn("")
			bc:sendToTag("DefenseLogic", "deadEnemy", "")
			bc:deactivate()
		end
		if call.label == "deactivate" then
			bc:despawn("")
			bc:deactivate()
		end
		call = bc:getCall()
	end
end

function Larva(bc)
	Larva_calls(bc)
	if bc:getRegInt("start") == 0 then Larva_start(bc) end
	bc:incRegInt("navCounter")
	if bc:getRegInt("navCounter") >= 120 then
		Larva_navigate(bc)
		bc:setRegInt("navCounter", 0)
	end
end