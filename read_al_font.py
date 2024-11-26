import sys
import itertools
import struct

from collections import namedtuple

class Header(namedtuple('Header', 'height proportional_baseline maxWidth')):
    @property
    def proportional(self):
        return self.proportional_baseline[0] >> 7

    @property
    def baseline(self):
        return self.proportional_baseline[0] & 0x7F


def main():
    with open(sys.argv[1], mode='rb') as f:
        fontData = f.read()

        header_format = "<Hch"

        header = Header._make(struct.unpack(header_format, fontData[:struct.calcsize(header_format)]))
        print(header)
        print(header.proportional, header.baseline)

        print(80 * '=')

        pointers = [a[0] for a in struct.iter_unpack(">H", fontData[
            struct.calcsize("<Hch"):255
        ])]

        for a in pointers:
            print(format(a, "016b").replace("0", ".").replace("1", "#"))

        print(80 * '=')

        print("A offset = ", pointers[ord('A')])

        for a in struct.iter_unpack(">H", fontData[
                struct.calcsize("<Hch")+401:len(fontData)
            ]):
            print(format(a[0], "016b").replace("0", " ").replace("1", "#"))



if __name__ == '__main__':
    main()
