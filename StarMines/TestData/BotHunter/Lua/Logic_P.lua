function Logic_P(bc)
	local n = sceneName()

	if n == "Alt" then
		bc:setPosition(750, 300)
		bc:setText("YOU R A WINNER!")
	end

	if n == "Main" then
		bc:spawn("RedGuy", 400, 300, "", "default")
		bc:setRegInt("score", 0)
	end
end