print("cDefs.py creating definitions...")

import sys
import random

from ctypes import *
sm = cdll.LoadLibrary("StarMines.exe")

CIntAry = c_int * 10

class Behavior(object):
    def __init__(self):
        sm.Behavior_getCurrentComponent.restype = c_void_p
        
        sm.Behavior_setRegisterInt.argtypes = [c_void_p, c_int, c_int]

        sm.Behavior_getRegisterInt.argtypes = [c_void_p, c_int]
        sm.Behavior_getRegisterInt.restype = c_int

        sm.Behavior_setRegisterFloat.argtypes = [c_void_p, c_int, c_float]

        sm.Behavior_getRegisterFloat.argtypes = [c_void_p, c_int]
        sm.Behavior_getRegisterFloat.restype = c_float

        sm.Behavior_spawn.argtypes = [c_void_p, c_char_p, c_float, c_float,
                                      CIntAry, c_int]

        sm.Behavior_deactivate.argtypes = [c_void_p]
        
        self.obj = sm.Behavior_getCurrentComponent()

    def deactivate(self):
        sm.Behavior_deactivate(self.obj)
    
    def getRegisterInt(self, i):
        return sm.Behavior_getRegisterInt(self.obj, i)

    def setRegisterInt(self, i, val):
        sm.Behavior_setRegisterInt(self.obj, i, val)

    def getRegisterFloat(self, i):
        return sm.Behavior_getRegisterFloat(self.obj, i)

    def setRegisterFloat(self, i, val):
        sm.Behavior_setRegisterFloat(self.obj, i, val)

    def spawn(self, blueprint, x, y, data, dataSz):
        sm.Behavior_spawn(self.obj, blueprint, x, y, data, dataSz)


print("cDefs.py complete.")
