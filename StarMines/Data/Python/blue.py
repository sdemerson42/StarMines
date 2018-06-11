import math

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

    counter = b.getRegisterInt(0)
    counter += 1
    if counter == 15:
        b.setRegisterInt(0,0)
        move(b)
    else:
        b.setRegisterInt(0,counter)

    #bounds
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y

    if x > 770:
        x = 770
    if x < 0:
        x = 0
    if y > 570:
        y = 570
    if y < 0:
        y = 0

    b.setPosition(x,y)

def move(b):
    
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y

    b.setTargetByTag(b'Red', b'near')
    
    tpos = b.targetPosition()

    if not tpos:
        return

    u = tpos.contents.x
    v = tpos.contents.y

    if math.fabs(u-x) > 50 or math.fabs(v-y) > 50:
        b.setDirection(0,0)
        return

    b.setSpeed(3)
    b.setDirection(u-x, v-y)


def infect(b):
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y

    b.spawn(b'Red', x, y, None, 0)
    b.despawn(None, 0)

    b.playSound(b'infect',40,False,False)
    b.deactivate()

behave()
