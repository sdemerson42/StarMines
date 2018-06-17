-- Persist - Executed on scene change to allow persistent behaviors.

-- Persist execution loop
local g_count = getBCSize()
for g_i = 0, g_count-1 do
	local bc = getBC(g_i)
	if bc:active() and bc:gPersist() then
		local module = bc:module() .. "_P"
		local path = "data/lua/" .. module
		require(path)
		_G[module](bc)
	end
end
