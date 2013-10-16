
class SimpleMeshParser(object):
    START_BYTE = '\xAB'
    MAX_PAYLOAD_SIZE = 124
    COMMAND_TYPES = { 'data_request' : '\x20' }
    
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
        
        
