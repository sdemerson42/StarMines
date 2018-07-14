function SentryShot(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "init" then
			local data = stoary(call.sdata)
			bc:setDir(data[1], data[2])
			bc:setSpeed(4)
		end
		if call.label == "collision" and bc:callerHasTag("Wave") then
			bc:despawn("")
		end
		
		call = bc:getCall()
	end

	bc:incRegInt("life")
	if bc:getRegInt("life") == 60 then
		bc:despawn("")
	end
	
end