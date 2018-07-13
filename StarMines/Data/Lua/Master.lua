function Master_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		
		call = bc:getCall()
	end
end

function Master(bc)
	Master_calls(bc)
end