function SentryIcon_fire(bc)
	local pos = bc:position()
	bc:playSound("Jump", 40, true, false)
	local tx = bc:getRegInt("targetX") - pos.x
	local ty = bc:getRegInt("targetY") - pos.y
	bc:spawn("SentryShot", pos.x + 4, pos.y + 4, tx..","..ty, "default")
end

function SentryIcon_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "init" then
			bc:setRegInt("index", stoary(call.sdata)[1])
		end
		if call.label == "target" then
			local data = stoary(call.sdata)
			if bc:getRegInt("index") == data[1] then
				bc:setRegInt("targetX", data[2])
				bc:setRegInt("targetY", data[3])
				bc:setRegInt("hasTarget", 1)
			end
		end
		if call.label == "collision" and bc:callerHasTag("Wave") then
			bc:playSound("GameOver", 40, true, false)
			bc:despawn("")
			masterSentries[bc:getRegInt("index")].active = false
		end
		call = bc:getCall()
	end
end

function SentryIcon(bc)
	bc:setRegInt("hasTarget", 0)

	SentryIcon_calls(bc)
	
	bc:incRegInt("shootCooldown")
	if bc:getRegInt("shootCooldown") > 24 then
		bc:setRegInt("shootCooldown", 25)
		if bc:getRegInt("hasTarget") == 1 then
			bc:setRegInt("shootCooldown", 0)
			SentryIcon_fire(bc)
		end
	end 
end