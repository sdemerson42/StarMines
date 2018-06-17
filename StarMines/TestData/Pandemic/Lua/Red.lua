function Red_change(bc)
	local r = math.random(1,4)
	if r == 1 then bc:setDir(1,0) end
	if r == 2 then bc:setDir(-1,0) end
	if r == 3 then bc:setDir(0,1) end
	if r == 4 then bc:setDir(0,-1) end
	bc:setRegInt(0, math.random(20,50))
end

function Red_bounds(bc)
	local pos = bc:position()
	local x = pos.x
	local y = pos.y
	if x > 770 then x = 770 Red_change(bc) end
	if x < 0 then x = 0 Red_change(bc) end
	if y > 570 then y = 570 Red_change(bc) end
	if y < 0 then y = 0 Red_change(bc) end
	bc:setPosition(x,y)
end

function Red_move(bc)
	local counter = bc:getRegInt(0)
	if counter == 0 then
		Red_change(bc)
	else
		bc:setRegInt(0, counter - 1)
	end
	
	Red_bounds(bc)
end

function Red(bc)
	bc:setSpeed(5)
	Red_move(bc)
end