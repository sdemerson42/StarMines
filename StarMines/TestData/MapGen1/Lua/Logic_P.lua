function Logic_P(bc)
	local n = sceneName()

	if n == "Alt" then
		bc:setPosition(750, 300)
		bc:setText("YOU R A WINNER!")
	end

	if n == "Main" then
		bc:setRegInt("score", 0)
		bc:spawn("RedGuy", 928, 448, "", "default")
	end
end