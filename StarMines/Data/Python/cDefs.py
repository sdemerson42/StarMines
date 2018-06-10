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

class Input(Structure):
    _fields_ = [("x", c_float),
                ("y", c_float),
                ("u", c_float),
                ("v", c_float)]
    
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
        sm.Behavior_despawn.argtypes = [c_void_p, CIntAry, c_int]

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

        sm.Behavior_setDirection.argtypes = [c_void_p, c_float, c_float]
        sm.Behavior_direction.argtypes = [c_void_p]
        sm.Behavior_direction.restype = POINTER(Vector2)

        sm.Behavior_setSpeed.argtypes = [c_void_p, c_float]
        sm.Behavior_speed.argtypes = [c_void_p]
        sm.Behavior_speed.restype = c_float

        sm.Behavior_playAnimation.argtypes = [c_void_p, c_char_p]

        sm.Behavior_setTargetBySender.argtypes = [c_void_p, c_void_p]
        sm.Behavior_setTargetByTag.argtypes = [c_void_p, c_char_p, c_char_p]
        sm.Behavior_targetPosition.argtypes = [c_void_p]
        sm.Behavior_targetPosition.restype = POINTER(Vector2)

        sm.Behavior_input.argtypes = [c_void_p]
        sm.Behavior_input.restype = POINTER(Input)

        sm.Behavior_playSound.argtypes = [c_void_p, c_char_p, c_float,
                                          c_bool, c_bool]
        sm.Behavior_stopSound.argtypes = [c_void_p, c_char_p]

        sm.Behavior_setText.argtypes = [c_void_p, c_char_p]
        sm.Behavior_appendText.argtypes = [c_void_p, c_char_p]

        sm.Behavior_newScene.argtypes = [c_void_p, c_char_p]
        sm.Behavior_sendSceneSpawnData.argtypes = [c_void_p,
                                                   CIntAry, c_int]
        
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

    def despawn(self, data, dataSz):
        sm.Behavior_despawn(self.obj, data, dataSz)

    def getCall(self):
        return sm.Behavior_getCall(self.obj)

    def sendToCaller(self, caller, message, data, dataSz):
        sm.Behavior_sendToCaller(self.obj, caller, message, data, dataSz)

    def sendToTag(self, tag, message, data, dataSz):
        sm.Behavior_sendToTag(self.obj, tag, message, data, dataSz)

    def setDirection(self, x, y):
        sm.Behavior_setDirection(self.obj, x, y)

    def direction(self):
        return sm.Behavior_direction(self.obj)

    def setSpeed(self, sp):
        sm.Behavior_setSpeed(self.obj, sp)

    def speed(self):
        return sm.Behavior_speed(self.obj)

    def playAnimation(self, anim):
        sm.Behavior_playAnimation(self.obj, anim)

    def setTargetBySender(self, entity):
        sm.Behavior_setTargetBySender(self.obj, entity)

    def setTargetByTag(self, tag, searchMethod):
        sm.Behavior_setTargetByTag(self.obj, tag, searchMethod)

    def targetPosition(self):
        return sm.Behavior_targetPosition(self.obj)

    def input(self):
        return sm.Behavior_input(self.obj)

    def playSound(self, tag, volume, hi, loop):
        sm.Behavior_playSound(self.obj, tag, volume, hi, loop)

    def stopSound(self, tag):
        sm.Behavior_stopSound(self.obj, tag)

    def setText(self, s):
        sm.Behavior_setText(self.obj, s)

    def appendText(self, s):
        sm.Behavior_appendText(self.obj, s)

    def newScene(self, scene):
        sm.Behavior_newScene(self.obj, scene)

    def sendSceneSpawnData(self, data, sz):
        sm.Behavioe_sendSceneSpawnData(self.obj, data, sz)
    


print("cDefs.py complete.")
