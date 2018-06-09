b = Behavior()

data = CIntAry()
for i in range(1,51):
    b.spawn(b"Blue", float(random.randint(0, 500)),
             float(random.randint(0, 500)), data, 1)

for i in range(1,11):
    b.spawn(b"Green", float(random.randint(0, 500)),
               float(random.randint(0, 500)), data, 1)

b.spawn(b"Red", float(random.randint(0, 500)),
         float(random.randint(0, 500)), data, 1)

b.deactivate()
