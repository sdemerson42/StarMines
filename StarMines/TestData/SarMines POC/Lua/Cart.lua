function Cart_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and bc:callerHasTag("Wave") then
			bc:playSound("GameOver", 40, true, false)
			bc:despawn("")
			g_cart = g_cart - 1
			masterMines[bc:getRegInt("index")].mined = false
			bc:sendToTag("FoundryIcon", "deactivate", bc:getRegFloat("x")..","..bc:getRegFloat("y"))
		end
		if call.label == "endWave" then
			bc:setSpeed(bc:speed() * 5)
		end
		if call.label == "deactivate" then
			bc:despawn("")
		end
		call = bc:getCall()
	end
end

function Cart_target(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "init" then
			local index = stoary(call.sdata)
			bc:setRegFloat("x", masterMines[index[1]].x)
			bc:setRegFloat("y", masterMines[index[1]].y)
			bc:incRegInt("phase")
			bc:setRegInt("index", index[1])
			bc:playAnim("empty")
		end
		call = bc:getCall()
	end
end

function Cart_go(bc)
	local x = bc:getRegFloat("x")
	local y = bc:getRegFloat("y")
	local pos = bc:position()
	bc:setDir(x - pos.x, y - pos.y)
	bc:setSpeed(1)
	bc:incRegInt("phase")
end

function Cart_wait(bc)
	local pos = bc:position()
	local tx = bc:getRegFloat("x")
	local ty = bc:getRegFloat("y")
	if math.abs(pos.x - tx) < 5 and math.abs(pos.y - ty) < 5 then
		bc:playAnim("full")
		bc:setDir(540 - pos.x, 540 - pos.y)
		bc:incRegInt("phase")	
	end
end

function Cart_return(bc)
	local pos = bc:position()
	local tx = 540
	local ty = 540
	if math.abs(pos.x - tx) < 5 and math.abs(pos.y - ty) < 5 then
		bc:playAnim("empty")
		bc:setDir(bc:getRegFloat("x") - pos.x, bc:getRegFloat("y") - pos.y)
		bc:setRegInt("phase", 2)
		g_ore = g_ore + 1
	end
end


function Cart(bc)
	local phase = bc:getRegInt("phase")
	if phase == 0 then
		Cart_target(bc)
	elseif phase == 1 then 
		Cart_go(bc)
	elseif phase == 2 then
		Cart_wait(bc)
	else
		Cart_return(bc)
	end
	Cart_calls(bc)
end