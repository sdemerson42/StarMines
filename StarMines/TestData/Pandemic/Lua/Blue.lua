function Blue_bounds(bc)
	local pos = bc:position()
	local x = pos.x
	local y = pos.y
	if x > 770 then x = 770 end
	if x < 0 then x = 0 end
	if y > 570 then y = 570 end
	if y < 0 then y = 0 end
	bc:setPosition(x,y)
end

function Blue_escape(bc)
	bc:setRegInt(0, 30)
	bc:setTargetByTag("Red", "near")
	local pos = bc:position()
	local tpos = bc:targetPosition()
	local x,y,tx,ty = pos.x, pos.y, tpos.x, tpos.y
	if math.abs(x - tx) > 100 or math.abs(y - ty) > 100 then bc:setSpeed(0) return end
	local dx,dy = 0,0
	if x < tx then dx = -1 end
	if x > tx then dx = 1 end
	if y < tx then dy = -1 end
	if y > ty then dy = 1 end
	bc:setDir(dx,dy)
	bc:setSpeed(3)
end

function Blue_move(bc)
	local counter = bc:getRegInt(0)
	if counter == 0 then Blue_escape(bc) end
	bc:setRegInt(0, bc:getRegInt(0) - 1)
	Blue_bounds(bc)
end

function Blue_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and call.tag == "Red" then
			local pos = bc:position()
			bc:spawn("Red", pos.x, pos.y, "")
			bc:despawn("")
			bc:playSound("Infect", 40, false, false)
			return
		end
		call = bc:getCall()
	end
end

function Blue(bc)
	Blue_calls(bc)
	Blue_move(bc)
end