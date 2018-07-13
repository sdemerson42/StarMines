function Sweep(bc)
	local angle = bc:getRegFloat("angle")
	angle = angle + 1.5
	if angle >= 360 then angle = 0 end
	bc:setRegFloat("angle", angle)
	bc:rotate(angle)

	local color = bc:color()

	if bc:getRegInt("init") == 0 then bc:incRegInt("init") bc:setColor(color.r, color.g, color.b, 60) end	

	if bc:getRegInt("colorDir") == 0 then
		if color.a > 0 then bc:setColor(color.r, color.g, color.b, color.a-1) end
		if color.a - 1 == 0 then bc:incRegInt("colorDir") end
	else
		if color.a < 60 then bc:setColor(color.r, color.g, color.b, color.a+1) end
		if color.a + 1 == 60 then bc:decRegInt("colorDir") end
	end
end