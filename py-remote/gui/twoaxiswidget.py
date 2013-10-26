import math
import time
import pygame

class TwoAxisWidget(object):
    def __init__(self, size = (100,100), pos_func = None):
        self.size = size
        self.color = (32,128,32)
        self.axis_color = (64,64,64)
        self.pos = (size[0] / 2, size[1] / 2)
        self.radius = 16
        self.width = 4
        self.speed = 200
        self.pos_func = pos_func
        self.centered_rect = (self.radius,
                              self.radius,
                              size[0] - (self.radius * 2),
                              size[1] - (self.radius * 2))
        self.time = 0
        self._surface = pygame.Surface(size)

    def move(self):
        if not self.pos_func:
            self.time = time.time() * self.speed
            x = math.sin(self.time / 360.0 * math.pi)
            y = math.cos(self.time / 360.0 * math.pi)
        else:
            x, y = self.pos_func()
            
        x = (self.centered_rect[0] + self.centered_rect[2] * (x + 1) / 2)
        y = (self.centered_rect[1] + self.centered_rect[3] * (y + 1) / 2)

        self.pos = (int(round(x)), int(round(y)))
        
    def draw(self):
        self._surface.fill((32,32,32))
        pygame.draw.line(self._surface,
                         self.axis_color,
                         (0, self.size[1] / 2),
                         (self.size[0], self.size[1] / 2))
        pygame.draw.line(self._surface,
                         self.axis_color,
                         (self.size[0] / 2, 0),
                         (self.size[0] / 2, self.size[1]))
        pygame.draw.circle(self._surface,
                           self.color,
                           self.pos,
                           self.radius,
                           self.width)
        return self._surface
