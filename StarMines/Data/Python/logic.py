def behave():
    s = 3
    b = Behavior()

    dataT = c_int * 0
    data = dataT()

    for i in range(1,51):
        b.spawn(b"Blue", float(random.randint(0, 500)),
                 float(random.randint(0, 500)), data, 0)

    for i in range(1,11):
        b.spawn(b"Green", float(random.randint(0, 500)),
                   float(random.randint(0, 500)), data, 0)

    b.spawn(b"Red", float(random.randint(0, 500)),
             float(random.randint(0, 500)), data, 0)

    b.deactivate()

behave()

