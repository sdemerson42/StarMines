function Green_bounds(bc)
	local pos = bc:position()
	local x = pos.x
	local y = pos.y
	if x > 770 then x = 770 end
	if x < 0 then x = 0 end
	if y > 570 then y = 570 end
	if y < 0 then y = 0 end
	bc:setPosition(x,y)
end

function Green_move(bc)
	bc:setSpeed(1)
	bc:setDir(math.random(-1,1), math.random(-1,1))
	Green_bounds(bc)
end

function Green_calls(bc)
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

function Green(bc)
	Green_calls(bc)
	Green_move(bc)
end