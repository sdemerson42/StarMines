function Logic(bc)
	for i=1,30 do
		local x = math.random(0,770)
		local y = math.random(0, 570)
		bc:spawn("Green", x, y, "")
	end

	for i=1,40 do
		local x = math.random(0,770)
		local y = math.random(0, 570)
		bc:spawn("Blue", x, y, "")
	end

	local x = math.random(0,770)
	local y = math.random(0, 570)
	bc:spawn("Red", x, y, "")

	bc:deactivate()
end