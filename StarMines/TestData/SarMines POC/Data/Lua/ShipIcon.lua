function ShipIcon_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and bc:callerHasTag("Wave") then
			if bc:getRegInt("dmgCooldown") == 0 then
				bc:setRegInt("dmgCooldown", 50)
				bc:playSound("GameOver", 40, true, false)
				g_shipHp = g_shipHp - 1
				if g_shipHp == 0 then
					-- GAME OVER!!
					bc:despawn("")
					bc:deactivate()
					bc:sendToTag("SceneManager", "gameover", "0")
				end
			end
		end
		call = bc:getCall()
	end
end

function ShipIcon(bc)
	ShipIcon_calls(bc)
	if bc:getRegInt("dmgCooldown") > 0 then bc:decRegInt("dmgCooldown") end
end