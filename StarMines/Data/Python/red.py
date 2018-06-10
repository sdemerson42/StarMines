def behave():
    b = Behavior()

    p = b.position()
    p.contents.x += .1
    p.contents.y += .1

    b.setPosition(p.contents.x, p.contents.y)

behave()
