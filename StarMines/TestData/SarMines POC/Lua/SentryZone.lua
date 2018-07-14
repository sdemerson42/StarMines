function SentryZone(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		
		if call.label == "init" then
			local ary = stoary(call.sdata)
			bc:setRegInt("sentryIndex", ary[1])
		end

		if call.label == "collision" and bc:callerHasTag("Wave") == true then
			bc:setTargetByCaller()
			local pos = bc:targetPosition()
			bc:sendToTag("SentryIcon", "target", bc:getRegInt("sentryIndex")..","..pos.x..","..pos.y)
		end
		call = bc:getCall()
	end
end