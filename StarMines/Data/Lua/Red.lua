function Red(bc)
	if bc:getRegInt(0) == 1 then
		return
	end

	bc:setDir(1.0, 0.2)
	bc:setSpeed(3.0)
	bc:setRegInt(0,1)

end