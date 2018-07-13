function BotSpawnPoint_spawn(bc)
	local pos = bc:position()
	if bc:getRegInt("cells") == 1 then
		local r = math.random(1,6)
		if r == 1 then
			bc:spawn("RedCell", pos.x, pos.y, "", "none")
		else
			bc:spawn("BlueCell", pos.x, pos.y, "", "none")
		end

	else
		local upper = math.floor(1 + .5 * g_turn)
		if upper > 6 then upper = 6 end
		bc:spawn("Bot", pos.x, pos.y, math.random(1,upper), "none")
	end
	bc:setRegInt("cooldown", 60)
end

function BotSpawnPoint_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and call.tag == "Fire" then
			bc:playSound("GameOver", 40, false, false)
			bc:despawn("")
			bc:sendToTag("MineLogic", "deadPoint", "")
		end
		call = bc:getCall()
	end
end

function BotSpawnPoint(bc)
	if bc:getRegInt("reported") == 0 then
		bc:incRegInt("reported")
		bc:sendToTag("MineLogic", "report", "")
		local call = bc:getCall()
		local ary = stoary(call.sdata)
		bc:setRegInt("cells", ary[1])
	end

	BotSpawnPoint_calls(bc)
	local upper = 520 - 20 * g_turn
	if upper < 100 then upper = 100 end
	if math.random(1, upper) == 1 and bc:getRegInt("cooldown") == 0 then BotSpawnPoint_spawn(bc) end
	if bc:getRegInt("cooldown") > 0 then bc:decRegInt("cooldown") end
end