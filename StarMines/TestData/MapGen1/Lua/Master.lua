function Master_tryDir(bc, mapAry, dir, x, y)
	
	local t,b,l,r = 0,0,0,0

	if dir == "n" then
		t = y - 5
		b = y - 1
		l = x - 2
		r = x + 2
	end
	if dir == "s" then
		t = y + 1
		b = y + 5
		l = x - 2
		r = x + 2
	end
	if dir == "w" then
		t = y - 2
		b = y + 2
		l = x - 5
		r = x - 1
	end
	if dir == "e" then
		t = y - 2
		b = y + 2
		l = x + 1
		r = x + 5
	end


	if t < 1 or b > 30 or l < 1 or r > 58 then return false end

	for i = l, r do
		for j = t, b do
			if mapAry[i][j] ~= -1 then return false end
		end
	end


	for i = l-1, r+1 do
		for j = t-1, b+1 do
			if i == l-1 or i == r+1 or j == t-1 or j == b+1 then
				if mapAry[i][j] == 1 and not (i == l-1 and j == t-1) and not (i == l-1 and j == b+1) and not (i == r+1 and j == t-1) and not (i == r+1 and j == b+1) then
					mapAry[i][j] = 0
				else
					mapAry[i][j] = 1
				end
			else
				mapAry[i][j] = 0
			end
		end
	end

	-- Spawn Robot?

	if math.random(1,3) == 1 then bc:spawn("Bot", 32*(l+2), 32 * (t+2), "") end

	return true
end	

function Master_tryRoom(bc, mapAry)
	-- find a suitable hook

	local x,y = 0,0
	while mapAry[x][y] ~= 1 do
		x = math.random(0, 59)
		y = math.random(0, 31)
	end

	if x == 0 or y == 0 or x == 59 or y == 31 then return false end

	local built = false

	if mapAry[x][y+1] == 0 and mapAry[x][y-1] == -1 then built = Master_tryDir(bc, mapAry, "n", x, y) end
	if mapAry[x][y-1] == 0 and mapAry[x][y+1] == -1 then built = Master_tryDir(bc, mapAry, "s", x, y) end 
	if mapAry[x-1][y] == 0 and mapAry[x+1][y] == -1 then built = Master_tryDir(bc, mapAry, "e", x, y) end 
	if mapAry[x+1][y] == 0 and mapAry[x-1][y] == -1 then built = Master_tryDir(bc, mapAry, "w", x, y) end 

	return built	
	
end

function Master_buildMap(bc, mapAry)
	for i = 0, 59 do
		for j = 0, 31 do
			if mapAry[i][j] == 1 then
				bc:spawn("Wall", i * 32, j * 32, "")
			elseif mapAry[i][j] == 0 then
				bc:spawn("Floor", i * 32, j * 32, "")
			end
		end
	end
end

function Master_genMap(bc)
	-- Generate map array
	

	-- Create arrays
	local mapAry = {}
	for i = 0, 59 do
		mapAry[i] = {}
		for j = 0, 31 do
			mapAry[i][j] = -1
		end
	end

	-- Create first room

	for i = 26,32 do
		for j = 11, 17 do
			if i == 26 or j == 11 or i == 32 or j == 17 then mapAry[i][j] = 1
			else mapAry[i][j] = 0 end
		end
	end

	local roomCount = 1
	math.randomseed(os.time())

	while roomCount < 15 do
		if Master_tryRoom(bc, mapAry) then roomCount = roomCount + 1 end
	end
	
	-- Build map

	Master_buildMap(bc, mapAry)
	
	collectgarbage()

	bc:spawn("RedGuy", 928, 448, "")
end


function Master(bc)
	if bc:getRegInt("mapGeneratedFlag") == 0 then
		bc:incRegInt("mapGeneratedFlag")
		Master_genMap(bc) 
	end
end