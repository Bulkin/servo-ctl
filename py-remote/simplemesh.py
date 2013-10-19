
class SimpleMeshParser(object):
    START_BYTE = '\xAB'
    MAX_PAYLOAD_SIZE = 124
    COMMAND_TYPES = { 'data_request' : '\x20',
                      'set_uart_mode_request' : '\x05'}
    BAUDRATEDICT = { 0              :  '\x00',
                     50             :  '\x01',
                     75             :  '\x02',
                     110            :  '\x03',
                     150            :  '\x04',
                     300            :  '\x05',
                     1200           :  '\x06',
                     2400           :  '\x07',
                     4800           :  '\x08',
                     9600           :  '\x09',
                     19200          :  '\x0a',
                     38400          :  '\x0b',
                     57600          :  '\x0c',
                     115200         :  '\x0d',
                     230400         :  '\x0e',
                     460800         :  '\x0f',
                     2000           :  '\x10',
                     4000           :  '\x11',
                     8000           :  '\x12',
                     10000          :  '\x13',
                     20000          :  '\x14',
                     30000          :  '\x15',
                     40000          :  '\x16',
                     50000          :  '\x17',
                     60000          :  '\x18',
                     70000          :  '\x19',
                     80000          :  '\x1a',
                     90000          :  '\x1b',
                     100000         :  '\x1c',
                     200000         :  '\x1d',
                     300000         :  '\x1e',
                     400000         :  '\x1f',
    }

    
    def __init__(self, packet_counter = 0):
        self.packet_counter = packet_counter

    def _calc_crc(self, buf):
        crc = 0x1234
        for b in buf:
            data = ord(b)
            data ^= crc & 0xFF
            data ^= (data << 4) & 0xFF
            crc = ((((data << 8) & 0xFFFF) | ((crc >> 8) & 0xFF)) ^
                  ((data >> 4) & 0xFF) ^ ((data << 3) & 0xFFFF))

        return chr(crc & 0xFF) + chr((crc & 0xFF00) >> 8)


    def make_packet(self, payload):
        packet = self.START_BYTE
        if len(payload) > self.MAX_PAYLOAD_SIZE:
            raise RuntimeError('payload too big')
        packet += chr(len(payload))
        packet += payload
        packet += self._calc_crc(payload)
        return packet

    def make_send_data_cmd(self, addr, data):
        packet = self.COMMAND_TYPES['data_request']
        packet += chr(addr & 0xFF)
        packet += chr(addr >> 8)
        packet += chr(0)
        packet += chr(self.packet_counter)

        self.packet_counter = (self.packet_counter + 1) % 256

        packet += data
        return self.make_packet(packet)
 
    def make_reconfig_uart_cmd(self, baudrate):
        packet = self.COMMAND_TYPES['set_uart_mode_request']
        packet += '\x00'
        packet += '\x00'
        packet += '\x00'
        packet += self.BAUDRATEDICT[baudrate]

        return self.make_packet(packet)
        
