import pygame
from pygame.locals import *

class Renderer(object):
    def __init__(self, name, resolution = (640, 480), fullscreen = False):
        pygame.init()
        pygame.display.set_caption(name)
        mode_flags = fullscreen and pygame.FULLSCREEN
        self._screen = pygame.display.set_mode(resolution, mode_flags)
        self._running = True
        self._widgets = []
        self._background = pygame.Surface(self._screen.get_size()).convert()

        self._redraw()

    def __del__(self):
        pygame.quit()

    def _redraw(self):
        self._background.fill((0,0,0))
        for obj in self._widgets:
            self._background.blit(obj['widget'].draw(), 
                                  obj['pos'])
            
        self._screen.blit(self._background, (0,0))
        pygame.display.flip()

    def add_widget(self, widget, pos):
        self._widgets.append({'widget' : widget,
                              'pos'    : pos})

    def main_loop(self, tick_action):
        while self._running:
            for event in pygame.event.get():
                if event.type == QUIT:
                    return

            for obj in self._widgets:
                obj['widget'].move()

            self._redraw()
            tick_action()

