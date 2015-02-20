
import os
import struct

from pyvi import SerialTransport

settings = {'port': '/dev/ttyUSB0',
            'baudrate': 115200,
            'timeout': 10000}

V_RMS = 220
I_RMS = 10


def read_calibration_package(port, struct_str):
    pkg = port.read_package_from_xmega()
    pkg_raw = reduce(lambda s, c: s + chr(int(c)), pkg, "")
    v, i = struct.unpack(struct_str, pkg_raw)
    return v, i


def write_char(port):
    port.serial.write('c')


def ask_for_number(question):
    number = None
    while number is None:
        number = raw_input(question)
        if not number.isdigit():
            print("'{!r}' is not a number".format(number))
            number = None
    return number


CIRCUIT_DEFINE_TPL = '''

#define C{0}_V_OFFSET {1[v_offset]}
#define C{0}_I_OFFSET {1[i_offset]}
#define C{0}_V_GAIN {1[v_gain]}
#define C{0}_I_GAIN {1[i_gain]}
#define C{0}_DELAY {1[delay]}

'''


def write_header_file(board_number, calibration):
    fname = './board_calibration_data/pivy_{}.h'.format(board_number)
    with open(fname+'.tmp', 'w') as fout:
        for circuit_id in range(1, 7):
            fout.write(CIRCUIT_DEFINE_TPL.format(circuit_id,
                                                 calibration[circuit_id]))
    os.rename(fname+'.tmp', fname)
    print('calibration data writed to {}'.format(fname))

if __name__ == "__main__":
    port = SerialTransport()
    port.open(settings)

    board_number = ask_for_number('Insert the board number: ')
    calibration = {}
    for circuit_id in range(1, 7):
        calibration[circuit_id] = {}
        print('\n\nCalibrating the number circuit {}'.format(circuit_id))
        print('Measuring offset, disconnect V and I, and press any key')
        raw_input('')
        write_char(port)
        v_offset, i_offset = read_calibration_package(port, '<HH')
        print('v_offset = {}, i_offset = {}\n'.format(v_offset, i_offset))
        calibration[circuit_id]['v_offset'] = v_offset
        calibration[circuit_id]['i_offset'] = i_offset

        print('Measuring gain, connect V and I, and press any key')
        raw_input('')
        write_char(port)
        v_rms2, i_rms2 = read_calibration_package(port, '<II')
        print('v_rms2 = {}, i_rms2 = {}\n'.format(v_rms2, i_rms2))
        try:
            calibration[circuit_id]['v_gain'] = V_RMS / ((v_rms2**.5) / (1 << 12))
            calibration[circuit_id]['i_gain'] = I_RMS / ((i_rms2**.5) / (1 << 12))
        except:
            pass
        delay = ask_for_number('Insert the delay for V channel [us]: ')
        calibration[circuit_id]['delay'] = int(float(delay) * 4)

    print calibration

#    write_header_file(board_number, calibration)

