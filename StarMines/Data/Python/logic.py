def behave():
    b = Behavior()

    dataT = c_int * 0
    data = dataT()
    
    for i in range(1,51):
        b.spawn(b"Blue", float(random.randint(0, 770)),
                 float(random.randint(0, 570)), data, 0)
    
    for i in range(1,11):
        b.spawn(b"Green", float(random.randint(0, 770)),
                   float(random.randint(0, 570)), data, 0)

    b.spawn(b"Red", float(random.randint(0, 770)),
             float(random.randint(0, 570)), data, 0)
    x = 5
    s = "Score: "
    s += str(x)
    txt = bytes(s, 'utf-8')

    b.setText(txt)
    b.appendText(b'0')
    

    b.deactivate()

behave()

