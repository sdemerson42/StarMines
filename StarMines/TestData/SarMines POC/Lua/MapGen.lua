function MapGen_tryDir(bc, mapAry, dir, x, y, mapIndex)
	
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


	if t < 1 or b > 58 or l < 1 or r > 58 then return false end

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

	-- Spawner?

	if math.random(1,2) == 1 then
		bc:addSceneData("Mine"..mapIndex, "scene", false, 1, "BotSpawnPoint", 32*(l+2), 32 * (t+2), bc:getRegInt("cells"))
	end

	return true
end	

function MapGen_tryRoom(bc, mapAry, mapIndex)
	-- find a suitable hook

	local x,y = 0,0
	while mapAry[x][y] ~= 1 do
		x = math.random(0, 59)
		y = math.random(0, 59)
	end

	if x == 0 or y == 0 or x == 59 or y == 59 then return false end

	local built = false

	if mapAry[x][y+1] == 0 and mapAry[x][y-1] == -1 then built = MapGen_tryDir(bc, mapAry, "n", x, y, mapIndex) end
	if mapAry[x][y-1] == 0 and mapAry[x][y+1] == -1 then built = MapGen_tryDir(bc, mapAry, "s", x, y, mapIndex) end 
	if mapAry[x-1][y] == 0 and mapAry[x+1][y] == -1 then built = MapGen_tryDir(bc, mapAry, "e", x, y, mapIndex) end 
	if mapAry[x+1][y] == 0 and mapAry[x-1][y] == -1 then built = MapGen_tryDir(bc, mapAry, "w", x, y, mapIndex) end 

	return built	
	
end

function MapGen_buildMap(bc, mapAry, mapIndex)
	for i = 0, 59 do
		for j = 0, 59 do
			if mapAry[i][j] == 1 then
				if bc:getRegInt("cells") == 1 then
					bc:addSceneData("Mine"..mapIndex, "scene", false, 1, "BioWall", i*32, j*32, "")
				else
					bc:addSceneData("Mine"..mapIndex, "scene", false, 1, "Wall", i*32, j*32, "")
				end
			elseif mapAry[i][j] == 0 then
				bc:addSceneData("Mine"..mapIndex, "scene", false, 1, "Floor", i*32, j*32, "")
			end
		end
	end
end

function MapGen_genMap(bc, mapIndex)
	-- Generate map array
	

	-- Create arrays
	local mapAry = {}
	for i = 0, 59 do
		mapAry[i] = {}
		for j = 0, 59 do
			mapAry[i][j] = -1
		end
	end

	-- Create first room

	for i = 26,32 do
		for j = 26, 32 do
			if i == 26 or j == 26 or i == 32 or j == 32 then mapAry[i][j] = 1
			else mapAry[i][j] = 0 end
		end
	end

	local roomCount = 1
	local roomTot = math.random(15,30)

	while roomCount < roomTot do
		if MapGen_tryRoom(bc, mapAry, mapIndex) then roomCount = roomCount + 1 end
	end
	
	-- Build map

	MapGen_buildMap(bc, mapAry, mapIndex)
	
	collectgarbage()
end

function MapGen_calls(bc)
	local call = bc:getCall()
	while call.label ~= "nil" do
		if call.label == "generate" then
			bc:setRegInt("cells", math.random(1,2))
			local mapIndex = stoary(call.sdata)
			bc:createNewScene("Mine"..mapIndex[1], "1920,1920,96,96","800,600,.25,.25,.416,.555,400,300")
			bc:addSceneData("Mine"..mapIndex[1], "scene", false, 1, "MineLogic", 0, 0, "")
			bc:addSceneData("Mine"..mapIndex[1], "scene", false, 1, "RedGuy", 928, 928, "")
			MapGen_genMap(bc, mapIndex[1])
			print("Gen scene: " .. "Mine"..mapIndex[1]) 
		end
		call = bc:getCall()
	end
end

function MapGen(bc)
	MapGen_calls(bc)
end