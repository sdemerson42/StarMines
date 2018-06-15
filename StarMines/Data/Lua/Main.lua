-- Behavior execution loop

local count = getBCSize()
for i = 0, count-1 do
	local bc = getBC(i)
	local module = bc:module()
	local path = "data/lua/" .. module
	require(path)
	_G[module](bc)
end