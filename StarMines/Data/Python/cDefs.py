print("cDefs.py creating definitions...")

import sys
import random

from ctypes import *
sm = cdll.LoadLibrary("StarMines.exe")

CIntAry = POINTER(c_int)
CStrAry = POINTER(c_char_p)

class Vector2(Structure):
    _fields_ = [("x", c_float),
                ("y", c_float)]

class CCall(Structure):
    _fields_ = [("sender", c_void_p),
                ("label", c_char_p),
                ("data", POINTER(c_int)),
                ("sz", c_int)]

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

        sm.Behavior_position.argtypes = [c_void_p]
        sm.Behavior_position.restype = POINTER(Vector2)

        sm.Behavior_setPosition.argtypes = [c_void_p, c_float, c_float]

        sm.Behavior_getCall.argtypes = [c_void_p]
        sm.Behavior_getCall.restype = POINTER(CCall)

        sm.Behavior_sendToCaller.argtypes = [c_void_p, c_void_p, c_char_p, CIntAry,
                                             c_int]

        sm.Behavior_sendToTag.argtypes = [c_void_p, c_char_p, c_char_p, CIntAry,
                                             c_int] 
        
        self.obj = sm.Behavior_getCurrentComponent()
    
    def deactivate(self):
        sm.Behavior_deactivate(self.obj)
    
    def getRegisterInt(self, i):
        return sm.Behavior_getRegisterInt(self.obj, i)

    def setRegisterInt(self, i, val):
        sm.Behavior_setRegisterInt(self.obj, i, val)

    def getRegisterFloat(self, i):
        return sm.Behavior_getRegisterFloat(self.obj, i)

    def position(self):
        return sm.Behavior_position(self.obj)

    def setPosition(self, x, y):
        sm.Behavior_setPosition(self.obj, x, y)

    def setRegisterFloat(self, i, val):
        sm.Behavior_setRegisterFloat(self.obj, i, val)

    def spawn(self, blueprint, x, y, data, dataSz):
        sm.Behavior_spawn(self.obj, blueprint, x, y, data, dataSz)

    def getCall(self):
        return sm.Behavior_getCall(self.obj)

    def sendToCaller(self, caller, message, data, dataSz):
        sm.Behavior_sendToCaller(self.obj, caller, message, data, dataSz)

    def sendToTag(self, tag, message, data, dataSz):
        sm.Behavior_sendToTag(self.obj, tag, message, data, dataSz)


print("cDefs.py complete.")
