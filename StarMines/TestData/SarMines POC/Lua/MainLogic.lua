function MainLogic(bc)
	local xs ={}
	local ys = {}
	for i = 1,10 do
		local x = 540
		local y = 540
		
		local flag = true
		while flag == true do
			local inFlag = true				
			x = math.random(250, 820)
			y = math.random(320, 800)
			if math.abs(x - 540) < 100 and math.abs(y - 540) < 100 then inFlag = false end
			if #xs > 0 then
				for j = 1, #xs do
					if math.abs(x - xs[j]) < 50 and math.abs(y-ys[j]) < 50 then inFlag = false end
				end
			end
			if inFlag == true then flag = false end
		end
		bc:sendToTag("MapGen", "generate", i)
		table.insert(masterMines, Mine.new(i, x, y))
		print(masterMines[#masterMines].name .. ": " .. masterMines[#masterMines].x .. "," .. masterMines[#masterMines].y)
		table.insert(xs, x)
		table.insert(ys, y)
	end
		
	bc:sendToTag("SceneManager", "Strat", "")
	bc:deactivate()	
end