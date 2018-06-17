function Plasma_init(bc)
	local call = bc:getCall()
	local ary = stoary(call.sdata)
	bc:setDir(ary[1], ary[2])
	bc:setSpeed(6)
end

function Plasma(bc)
	if bc:getRegInt(0) == 0 then Plasma_init(bc) end
	if bc:getRegInt(0) == 20 then
		bc:despawn("")	
	end
	bc:incRegInt(0)
end