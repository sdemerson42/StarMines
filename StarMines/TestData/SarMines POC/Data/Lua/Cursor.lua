function Cursor(bc)
	local posFlag = false
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "collision" and (call.tag == "MineIcon" or call.tag == "FoundryIcon") then
			bc:setTargetByCaller()
			local pos = bc:targetPosition()
			bc:setRegInt("mineX", pos.x)
			bc:setRegInt("mineY", pos.y)

			if call.tag == "FoundryIcon" then
				bc:setRegInt("mineFlag", 0)
			else
				bc:setRegInt("mineFlag", 1)
			end
		end
		call = bc:getCall()
	end
end