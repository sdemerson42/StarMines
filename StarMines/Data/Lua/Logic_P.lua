function Logic_P(bc)
	local n = sceneName()

	if n == "Alt" then
		bc:setPosition(300, 300)
		bc:setText("YOU R A WINNER!")
	end

	if n == "Main" then
		bc:spawn("RedGuy", 400, 300, "")
		bc:setRegInt(1, 0)
	end
end