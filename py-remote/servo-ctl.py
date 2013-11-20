from gui import renderer
from gui import input_devs
from gui import twoaxiswidget
import time
import serial
from simplemesh import SimpleMeshParser

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
        #self.serial.close()
        time.sleep(0.1)
        self.serial.baudrate = baudrate
        #self.serial.open()

    def send_pos(self):
        pos = self.pos_func()
        x = int(-1 * pos[0] * self.max_val)
        y = int(-1 * pos[1] * self.max_val)

        cmd = 's ' + str(x) + ' ' + str(y) + '\n'
        cmd = self.simplemesh.make_send_data_cmd(00, cmd)
        self.serial.write(cmd)
        time.sleep(0.02)

def main():
    r = renderer.Renderer('servo control')
    analog_stick = input_devs.AnalogStick()
    commander = Commander(analog_stick.get_pos, uart_dev = '/dev/ttyUSB0')
    commander.reconfig_uart()
    #commander.serial.baudrate = 115200
    r.add_widget(twoaxiswidget.TwoAxisWidget((200,200), 
                                             analog_stick.get_pos),
                 (100, 100))
    #renderer.add_drawable(Ball((100,100,200,200)))
    r.main_loop(commander.send_pos)
    

if __name__ == '__main__':
    main()
