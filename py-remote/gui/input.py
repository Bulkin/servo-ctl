import pygame

class AnalogStick(object):
    def __init__(self, id = 0, axes = (0,1)):
        self.axes = axes
        self.joystick = pygame.joystick.Joystick(id)
        self.joystick.init()

    def get_pos(self):
        x = self.joystick.get_axis(self.axes[0])
        y = self.joystick.get_axis(self.axes[1])        
        return (x, y)
