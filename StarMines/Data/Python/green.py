def behave():
    b = Behavior()

    #check calls

    call = b.getCall()
    while call:
        if call.contents.label == b'infect':
            infect(b)
            break

        call = b.getCall()

    #movement
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y
    r = random.randint(1,4)
    if r == 1:
        x += 1.0
    if r == 2:
        x -= 1.0
    if r == 3:
        y += 1.0
    if r == 4:
        y -= 1.0

    if x > 770:
        x = 770
    if x < 0:
        x = 0
    if y > 570:
        y = 570
    if y < 0:
        y = 0

    b.setPosition(x,y)
    
    

def infect(b):
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y

    b.spawn(b'Red', x, y, None, 0)
    b.despawn(None, 0)

    b.playSound(b'infect',40,False,False)
    b.deactivate()

behave()
