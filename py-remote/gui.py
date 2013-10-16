import pygame
from pygame.locals import *

class App(object):
    def __init__(self, name = 'Servo control'):
        self.name = name
        pygame.init()
        pygame.display.set_caption(name)
        self._screen = pygame.display.set_mode((640, 480))
        self._running = True

        self._background = pygame.Surface(self._screen.get_size()).convert()
        self._background.fill((0,0,0))

        self._text = pygame.font.Font(None, 16).render('HW!', 1, (32,128,32))
        self._background.blit(self._text, (16, 16))

    def __del__(self):
        pygame.quit()

    def quit(self):
        self.__running = false

    def main_loop(self):
        while self._running:
            for event in pygame.event.get():
                if event.type == QUIT:
                    return
            self._screen.blit(self._background, (0, 0))
            pygame.display.flip()
            pygame.time.delay(10)


if __name__ == '__main__':
    App().main_loop()
