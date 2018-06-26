function RedGuy_P(bc)
	local n = sceneName()	
	
	if n == "Alt" then
		bc:stopSound("Flame")
		bc:deactivate("")
		bc:setSpeed(0)
		bc:playAnim("right")
		bc:setPosition(390, 200)
	end

end