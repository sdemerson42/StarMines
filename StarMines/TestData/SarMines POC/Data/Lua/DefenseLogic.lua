function DefenseLogic_setup(bc)
	bc:setRegInt("waveTotal", 20 + g_turn * 8)
	bc:setRegInt("waveRate", 32 - g_turn * 2)
	if bc:getRegInt("waveRate") < 10 then
		bc:setRegInt("waveRate", 10)
	end
	bc:setRegInt("dead", bc:getRegInt("waveTotal"))
	bc:incRegInt("phase")
	bc:setRegInt("timer", 3600)
end

function DefenseLogic_spawn(bc)
	local r = math.random(1,bc:getRegInt("waveRate"))
	if r == 1 then
		local deg = math.random(0, 359)
		local rad = math.rad(deg)
		local y = 380 * math.sin(rad) + 540
		local x = 380 * math.cos(rad) + 540
		bc:spawn("Larva", x, y, "", "default")
		bc:decRegInt("waveTotal")
		if bc:getRegInt("waveTotal") == 0 then
			bc:incRegInt("phase")
		end
	end
end

function DefenseLogic_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "deadEnemy" then
			bc:decRegInt("dead")
			if bc:getRegInt("dead") == 0 then DefenseLogic_setupEnd(bc) end
		end
		call = bc:getCall()
	end
end

function DefenseLogic_setupEnd(bc)
	bc:sendToTag("Cart", "endWave", "")
	bc:incRegInt("fast")
end

function DefenseLogic(bc)
	DefenseLogic_calls(bc)
	local phase = bc:getRegInt("phase")
	if phase == 0 then
		DefenseLogic_setup(bc)
	elseif phase == 1 then
		DefenseLogic_spawn(bc)
	end
	
	if bc:getRegInt("fast") == 1 then
		bc:setRegInt("timer", bc:getRegInt("timer") - 5)
	else
		bc:decRegInt("timer")
	end

	if bc:getRegInt("timer") >= 0 then
		bc:setText(bc:getRegInt("timer"))
	else
		bc:setText("0")
	end

	if bc:getRegInt("timer") <= 0 then
		bc:despawn("")
		bc:sendToTag("Cart", "deactivate", "")
		bc:spawn("StrategyLogic", 1000, 180, "", "default")
		bc:spawn("Cursor", 540, 540, "", "default")
		g_explores = g_red
		g_turn = g_turn + 1
		collectgarbage()
	end
end