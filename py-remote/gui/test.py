from renderer import Renderer
from input import AnalogStick
from twoaxiswidget import TwoAxisWidget

def main():
    analog_stick = AnalogStick()
    renderer = Renderer('Servo Control')
    renderer.add_widget(TwoAxisWidget(size = (200,200), 
                                      pos_func = analog_stick.pos_func),
                        (10,10))
    renderer.main_loop()
