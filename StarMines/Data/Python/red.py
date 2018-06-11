def behave():
    b = Behavior()
     
    if b.getRegisterInt(1) == 0:
        init(b)
    
    if b.getRegisterInt(0) == 0:
        switch(b)
    move(b)

    #check calls

    call = b.getCall()
    while call:
        if call.contents.label == b'collision':
            b.sendToCaller(call.contents.caller, b'infect', None, 0)
        call = b.getCall()

def move(b):
    pos = b.position()
    x = pos.contents.x
    y = pos.contents.y

    if x > 770:
        x = 770
        b.setPosition(x,y)
        switch(b)
    if x < 0:
        x = 0
        b.setPosition(x,y)
        switch(b)
    if y > 570:
        y = 570
        b.setPosition(x,y)
        switch(b)
    if y < 0:
        y = 0
        b.setPosition(x,y)
        switch(b)
    
    counter = b.getRegisterInt(0)
    counter -= 1
    b.setRegisterInt(0, counter)


def switch(b):
    b.setRegisterInt(0, random.randint(20,50))
    x = 0.0
    y = 0.0
    r = random.randint(0,3)
    if r == 0:
        x = -1.0
    if r == 1:
        x = 1.0
    if r == 2:
        y = -1.0
    if r == 3:
        y = 1.0

    b.setDirection(x,y)
    

def init(b):
    b.setRegisterInt(1,1)
    b.setSpeed(6)

behave()
