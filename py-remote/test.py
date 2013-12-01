from gui import *
import time
from simplemesh import SimpleMeshParser

def main():
    print map(ord, SimpleMeshParser(14).make_send_data_cmd(4, 'test'))
    engine = renderer.Renderer('Servo Control')
    analog_stick = input_devs.AnalogStick()
    engine.add_widget(twoaxiswidget.TwoAxisWidget(size = (200,200), 
                                                    pos_func = analog_stick.get_pos),
                        (10,10))
    engine.main_loop(lambda: time.sleep(0.001))
