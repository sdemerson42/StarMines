-- string to ary
function stoary(s)
	local index = 1
	local r = {}
	local i = string.find(s, ',')
	while i ~= nil do
		local vs = string.sub(s, 1, i-1)
		r[index] = tonumber(vs)
		s = string.sub(s, i + 1, -1)
		index = index + 1
		i = string.find(s, ',')
	end
	r[index] = tonumber(s)
	r[index+1] = nil
	return r
end

-- Behavior execution loop
local count = getBCSize()
for i = 0, count-1 do
	local bc = getBC(i)
	local module = bc:module()
	local path = "data/lua/" .. module
	require(path)
	_G[module](bc)
end




