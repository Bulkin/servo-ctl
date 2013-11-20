from renderer import Renderer
from input_devs import AnalogStick
from twoaxiswidget import TwoAxisWidget

def main():
    renderer = Renderer('Servo Control')
    renderer.add_widget(TwoAxisWidget(size = (200,200))
                        (150,150))
    renderer.main_loop()
