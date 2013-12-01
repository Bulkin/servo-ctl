import pygame

class AnalogStick(object):
    def __init__(self, id = 0, axes = ((0, 0), (0, 1))):
        self.axes = axes
        pygame.joystick.init()
        self.joysticks = [ pygame.joystick.Joystick(x) for x in
                           range(pygame.joystick.get_count()) ]
        for j in self.joysticks:
            j.init()

    def get_pos(self):
        x = self.joysticks[self.axes[0][0]].get_axis(self.axes[0][1])
        y = self.joysticks[self.axes[1][0]].get_axis(self.axes[1][1])        
        return (x, y)
