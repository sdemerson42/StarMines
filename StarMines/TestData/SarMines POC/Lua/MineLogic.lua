function MineLogic(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "kill" then bc:incRegInt("kill") end
		if call.label == "redDead" then
			bc:sendToTag("SceneManager", "Strat", "0")
			bc:setRegInt("kill", -100)
			g_red = g_red - 1
			bc:deactivate()
		end
		if call.label == "report" then
			bc:incRegInt("points")
		end
		if call.label == "deadPoint" then
			bc:decRegInt("points")
			if bc:getRegInt("points") == 0 then
				bc:sendToTag("SceneManager", "Strat", "1")
				bc:deactivate()
			end
		end
		call = bc:getCall()
	end

	--if bc:getRegInt("kill") >= 20 then
	--	bc:sendToTag("SceneManager", "Strat", "1")
	--	bc:deactivate()
	--end
end