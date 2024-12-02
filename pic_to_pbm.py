import sys
import struct

from collections import namedtuple


PicHeader = namedtuple('Header', 'width height x')

def roundUpToMultipleOf(multiple, number):
    """ Round up to a multiple of eigth
    >>> roundUpToMultipleOf(16, 7)
    16
    >>> roundUpToMultipleOf(16, 8)
    16
    >>> roundUpToMultipleOf(16, 9)
    16
    >>> roundUpToMultipleOf(16, 15)
    16
    >>> roundUpToMultipleOf(16, 20)
    32
    >>> roundUpToMultipleOf(16, 274)
    288
    """

    if number % multiple == 0:
        return number
    else:
        return number + multiple - number % multiple


def main(argv):
    if len(argv) < 2:
        print("please supply the .pic file name as an argument")
        exit(-1)

    if len(argv) < 3:
        print("please supply the output file name as an argument")
        exit(-1)

    with open(argv[1], mode='rb') as f:
        whole_file = f.read()

    header_format = ">HHH" # big endian three words (6 bytes)
    header = PicHeader._make(struct.unpack(header_format, whole_file[:6]))

    with open(argv[2], mode='wb') as output:
        # PIC files are rounded to 16 bits increments
        roundedWidth = roundUpToMultipleOf(16, header.width)
        output.write(f"P4\n{roundedWidth} {header.height}\n".encode('ASCII'))
        output.write(whole_file[6:]) # skip original header


if __name__ == '__main__':
    main(sys.argv)
