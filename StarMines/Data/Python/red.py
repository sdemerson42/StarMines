def behave():
    b = Behavior()
    i = b.input()

    b.setDirection(i.contents.x, i.contents.y)
    b.setSpeed(5)


behave()
