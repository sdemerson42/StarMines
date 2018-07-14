function SceneManager_spawn(bc)
	local scene = bc:getRegInt("scene")
	
	if scene == 0 then
		if bc:getRegInt("mineIndex") ~= 0 then
			masterMines[bc:getRegInt("mineIndex")].clear = true
		end
		for k,v in ipairs(masterMines) do
			if v.clear == false then 
				bc:spawn("MineIcon", v.x, v.y, "", "default")
			else
				if v.mined == true then
					bc:spawn("FoundryIcon", v.x, v.y, v.x..","..v.y, "default")
				else
					bc:spawn("FoundryIcon", v.x, v.y, "", "default")
				end
				--bc:spawn("Cart", 540, 540, k, "default")
			end
		end

		local i = 1
		-- Clean up sentry table...
		while i <= #masterSentries do
			if masterSentries[i].active == false then
				table.remove(masterSentries, i)
			else
				i = i + 1
			end
		end

		for k,v in ipairs(masterSentries) do
			bc:spawn("SentryIcon", v.x, v.y, k, "default")
			bc:spawn("SentryZone", v.x - 96, v.y - 96, k, "default")
		end

	end

	if scene == 2 then
		bc:deactivate()
		if bc:getRegInt("endState") == 0 then
			bc:spawn("LoseMessage", 600, 500, "", "default")
		else
			bc:spawn("WinMessage", 540, 500, "", "default")
		end
	end


	bc:decRegInt("phase")
end

function SceneManager(bc)
	
	if bc:getRegInt("phase") == 1 then SceneManager_spawn(bc) end	

	local call = bc:getCall()
	while call.label ~= "nil" do
		
		if call.label == "Strat" then
			bc:stopSound("Flame")
			bc:newScene("Strat")
			bc:setRegInt("scene", 0)
			local data = stoary(call.sdata)
			if data[1] == 0 then bc:setRegInt("mineIndex", 0) end
			bc:stopMusic()
			bc:playMusic("audio/mus.wav", 40.0, true)
			bc:incRegInt("phase")
		end

		if call.label == "Mine" then
			local index = stoary(call.sdata)
			bc:newScene("Mine"..index[1])
			bc:setRegInt("scene", 1)
			bc:setRegInt("mineIndex", index[1])
			bc:stopMusic()
			bc:playMusic("audio/ASyn.wav", 40.0, true)
			bc:incRegInt("phase")
		end
		if call.label == "gameover" then
			local ary = stoary(call.sdata)
			bc:setRegInt("endState", ary[1])
			bc:setRegInt("scene", 2)
			bc:incRegInt("phase")
			bc:newScene("End")
		end
	
		call = bc:getCall()
	end
end