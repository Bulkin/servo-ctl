import pygame
from pygame.locals import *
import math
import time
import serial
from simplemesh import SimpleMeshParser

class Ball(object):
    def __init__(self, rect = (0,0,100,100), pos_func = None):
        self.rect = rect
        self.color = (32,128,32)
        self.pos = (rect[0] + rect[2]/2, rect[1] + rect[3]/2)
        self.radius = 16
        self.width = 4
        self.speed = 200
        self.pos_func = pos_func
        self.centered_rect = (rect[0] + self.radius,
                              rect[1] + self.radius,
                              rect[2] - (self.radius * 2),
                              rect[3] - (self.radius * 2))
        self.time = 0
        self._surface = pygame.Surface(rect[2:])

    def move(self):
        if not self.pos_func:
            self.time = time.time() * self.speed
            x = math.sin(self.time / 360.0 * math.pi)
            y = math.cos(self.time / 360.0 * math.pi)
        else:
            x, y = self.pos_func()
        
        x = (self.centered_rect[0] + self.centered_rect[2] / 2 * x -
             self.radius)
        y = (self.centered_rect[1] + self.centered_rect[3] / 2 * y -
             self.radius)

        self.pos = (int(round(x)), int(round(y)))
        
    def draw(self, surface):
        self._surface.fill((32,32,32))
        pygame.draw.circle(self._surface,
                           self.color,
                           self.pos,
                           self.radius,
                           self.width)
        surface.blit(self._surface, self.rect[0:2])

class AnalogStick(object):
    def __init__(self, id = 0, axes = (0,1)):
        self.axes = axes
        self.joystick = pygame.joystick.Joystick(id)
        self.joystick.init()

    def get_pos(self):
        x = self.joystick.get_axis(self.axes[0])
        y = self.joystick.get_axis(self.axes[1])        
        return (x, y)

class App(object):
    def __init__(self, name = 'Servo control'):
        self.name = name
        pygame.init()
        pygame.display.set_caption(name)
        self._screen = pygame.display.set_mode((640, 480))
        self._running = True
        self._drawables = []

        self._background = pygame.Surface(self._screen.get_size()).convert()
        self._text = pygame.font.Font(None, 16).render('HW!', 1, (32,128,32))

        self._redraw()

    def __del__(self):
        pygame.quit()

    def _redraw(self):
        self._background.fill((0,0,0))
        self._background.blit(self._text, (16, 16))
        for obj in self._drawables:
            obj.draw(self._background)
        
        self._screen.blit(self._background, (0, 0))
        pygame.display.flip()
        
    def add_drawable(self, drawable):
        self._drawables.append(drawable)
        
    def quit(self):
        self._running = False

    def main_loop(self, tick_action):
        while self._running:
            for event in pygame.event.get():
                if event.type == QUIT:
                    return

            for obj in self._drawables:
                obj.move()

            tick_action()
            pygame.time.wait(50)
            self._redraw()

class Commander(object):
    def __init__(self, pos_func, 
                 uart_dev = '/dev/ttyUSB0', uart_speed = 115200):
        self.serial = serial.Serial(uart_dev, uart_speed)
        self.max_val = 100
        self.pos_func = pos_func
        self.simplemesh = SimpleMeshParser()

    def __del__(self):
        self.serial.close()

    def reconfig_uart(self, baudrate=9600):
        cmd = self.simplemesh.make_reconfig_uart_cmd(9600)
        self.serial.write(cmd)
        self.serial.close()
        time.sleep(0.01)
        self.serial.baudrate = baudrate
        self.serial.open()

    def send_pos(self):
        pos = self.pos_func()
        x = int(-1 * pos[0] * self.max_val)
        y = int(-1 * pos[1] * self.max_val)

        cmd = 's 0 ' + str(x) + '\n'
        cmd = self.simplemesh.make_send_data_cmd(00, cmd)
        self.serial.write(cmd)
        time.sleep(0.05)
        cmd = 's 1 ' + str(y) + '\n'
        cmd = self.simplemesh.make_send_data_cmd(00, cmd)
        self.serial.write(cmd)
        

def main():
    renderer = App()
    analog_stick = AnalogStick()
    commander = Commander(analog_stick.get_pos, uart_dev = '/dev/ttyUSB2')
    #commander.reconfig_uart()
    renderer.add_drawable(Ball((100,100,200,200), analog_stick.get_pos))
    #renderer.add_drawable(Ball((100,100,200,200)))
    renderer.main_loop(commander.send_pos)
    

if __name__ == '__main__':
    main()
