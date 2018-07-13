function StrategyLogic_Input(bc)
	local input = bc:input()
	local phase = bc:getRegInt("phase")
	
	if phase == 0 then
		bc:incRegInt("phase") 
		bc:setTargetByTag("Cursor", "first")
		bc:setText("Turn: "..g_turn)
	
	elseif phase == 1 then
		local b = bc:targetBehavior()
		b:setDir(input.xAxis, input.yAxis)
		b:setSpeed(5)

		if input.a == true and g_red > 0 and b:getRegInt("mineFlag") == 1 and g_explores > 0 then
			local pos = b:position()
			for k,v in ipairs(masterMines) do
				if v.x == b:getRegInt("mineX") and v.y == b:getRegInt("mineY") then
					bc:playSound("Alarm", 40, true, false)
					bc:sendToTag("SceneManager", "Mine", k)
					g_explores = g_explores - 1
					bc:deactivate()
				end
			end
		end

		if input.b == true then
			if bc:getRegInt("bFlag") == 0 then
				bc:incRegInt("bFlag")
				-- Buy sentry if possible

				if g_ore >= 5 then
					g_ore = g_ore - 5
					local pos = b:position()
					table.insert(masterSentries, Sentry.new(pos.x, pos.y))
					bc:spawn("SentryIcon", pos.x, pos.y, #masterSentries, "default")
					bc:spawn("SentryZone", pos.x - 96, pos.y - 96, #masterSentries, "default")
					bc:playSound("Coin", 40, true, false)
				end
			end
		else
			bc:setRegInt("bFlag", 0)
		end

		if input.x == true then
			if bc:getRegInt("xFlag") == 0 then
				bc:incRegInt("xFlag")
				-- Buy cart if possible

				if g_ore >= 5 then
					local pos = b:position()
					for k,v in ipairs(masterMines) do
						if v.x == b:getRegInt("mineX") and v.y == b:getRegInt("mineY") and b:getRegInt("mineFlag") == 0 and v.mined == false then
							-- Purchase or set Route
							v.mined = true
							g_ore = g_ore - 5
							g_cart = g_cart + 1
							bc:sendToTag("FoundryIcon", "activate", v.x..","..v.y)
							bc:playSound("Coin", 40, true, false)
						end
					end
				end
			end
		else
			bc:setRegInt("xFlag", 0)
		end

		if input.y == true then
			if bc:getRegInt("yFlag") == 0 then
				bc:incRegInt("yFlag")
				if g_ore >= 10 then
					g_ore = g_ore - 10
					g_red = g_red + 1
					g_explores = g_explores + 1
					bc:playSound("Coin", 40, true, false)
				end
			end
		else
			bc:setRegInt("yFlag", 0)
		end

		if input.start == true then
			if bc:getRegInt("startFlag") == 0 then
				bc:setRegInt("phase", 2)
				b:despawn("")
				for k,v in ipairs(masterMines) do
					if v.mined == true then
						bc:spawn("Cart", 540, 540, k, "default")
					end
				end
				bc:spawn("DefenseLogic", 1120, 900, "", "none")
				bc:playSound("Alarm", 60, true, false)
				bc:despawn("")
			end
		else
			bc:setRegInt("startFlag", 0)
		end

		if input.select == true then
			if bc:getRegInt("selectFlag") == 0 then
				bc:incRegInt("selectFlag")
				if g_ore >= 20 and g_shipHp < 20 then
					g_shipHp = g_shipHp + 1
					g_ore = g_ore - 20
					bc:playSound("Coin", 40, true, false)
					if g_shipHp == 20 then
						bc:deactivate()
						bc:sendToTag("SceneManager", "gameover", "1")
					end
				end
			end
		else
			bc:setRegInt("selectFlag", 0)
		end
	else
		-- next phase
	
	end
	
end

function StrategyLogic(bc)
	StrategyLogic_Input(bc)
end