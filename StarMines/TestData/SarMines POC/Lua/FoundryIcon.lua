function FoundryIcon(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "activate" or call.label == "init" then
			local pos = bc:position()
			local ary = stoary(call.sdata)
			if pos.x == ary[1] and pos.y == ary[2] then 
				bc:setColor(10, 255, 50, 255)
			end
		end
		if call.label == "deactivate" then
			local pos = bc:position()
			local ary = stoary(call.sdata)
			if pos.x == ary[1] and pos.y == ary[2] then 
				bc:setColor(255, 255, 255, 255)
			end
		end 
		
		call = bc:getCall()
	end
end