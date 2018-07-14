Mine = {}
function Mine.new(index, _x, _y)
	return {name = "Mine"..index, x = _x, y = _y, clear = false, mined = false }
end

Sentry = {}
function Sentry.new(_x, _y)
	return { x = _x, y = _y, active = true }
end

function Master_P_init(bc)
	math.randomseed(os.time())
	masterMines = {}
	--masterSentries = { Sentry.new(500, 500), Sentry.new(580, 580), Sentry.new(500, 580), Sentry.new(580, 500) }
	masterSentries = {}
	g_ore = 20
	g_red = 1
	g_cart = 0
	g_usedCart = 0
	g_explores = g_red
	g_turn = 1
	g_shipHp = 5
	bc:incRegInt("init")
end

function Master_P(bc)
	if bc:getRegInt("init") == 0 then Master_P_init(bc) end
end