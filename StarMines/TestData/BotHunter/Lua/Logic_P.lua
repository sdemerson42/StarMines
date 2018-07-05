function Logic_P(bc)
	local n = sceneName()

	if n == "Alt" then
		bc:setPosition(750, 300)
		bc:setText("YOU R A WINNER!")
		bc:stopMusic()
	end

	if n == "Main" then
		if bc:getRegInt("music") == 0 then
			bc:incRegInt("music")
			bc:playMusic("audio/music.wav", 50.0, true)
		end
		bc:spawn("RedGuy", 400, 300, "", "default")
		bc:setRegInt("score", 0)
	end
end