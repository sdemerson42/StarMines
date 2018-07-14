function BlueCell_change(bc)
	local r = math.random(1,4)
	if r == 1 then bc:setDir(1,0) end
	if r == 2 then bc:setDir(-1,0) end
	if r == 3 then bc:setDir(0,1) end
	if r == 4 then bc:setDir(0,-1) end
	bc:setRegInt("moveCounter", math.random(20,50))
end


function BlueCell_move(bc)
	local counter = bc:getRegInt("moveCounter")
	if counter == 0 then
		BlueCell_change(bc)
	else
		bc:decRegInt("moveCounter")
	end
	
end

function BlueCell_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" then
			if call.tag == "Fire" then
				bc:playSound("CellDie", 40, false, false)
				bc:deactivate()
				bc:despawn("")
				bc:sendToTag("MineLogic", "deadPoint", "")
			elseif call.tag ~= "BotSpawnPoint" then 
				BlueCell_change(bc)
			end
		end
		call = bc:getCall()
	end
end

function BlueCell(bc)
	BlueCell_calls(bc)
	bc:setSpeed(2)
	BlueCell_move(bc)
end