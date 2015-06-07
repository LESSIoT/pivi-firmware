# This file is part of the Pivi firmware package.
# Released under the Creative Commons ATT-NC-ShareAlire license
# http://creativecommons.org/licenses/by-nc-sa/4.0/
# Copyright (C) 2014, 2015 LESS Industries S.A.
# Author: Diego Mascialino <dmascialino@gmail.com>

import os
import struct
import pickle
from pyvi import SerialTransport

settings = {'port': '/dev/ttyUSB0',
            'baudrate': 115200,
            'timeout': 10000}

V_RMS = 220
I_RMS = 10

def ask_for_circuits_to_calibrate():
    list_of_ids = []
    reply = 'y'
    circuit_id = ask_for_id('Insert one ID of a circuit you want to calibrate:')
    list_of_ids.append(circuit_id)
    reply=ask_for_y_or_n("Want to check more circuits? (y or n)")
    while reply == 'y':
            circuit_id = ask_for_id('Insert one ID of a circuit you want to calibrate:')
            if circuit_id not in list_of_ids:
                    list_of_ids.append(circuit_id)
            reply=ask_for_y_or_n("Want to check more circuits? (y or n)")
    list_of_ids.sort()
    return list_of_ids


def ask_for_y_or_n(question):
    reply = None
    while reply is None:
            reply = raw_input(question)
            if not (reply == 'y' or reply == 'n'):
                    print("{} is not a valid answer".format(reply))
                    reply = None
    return reply


def ask_for_id(question):
    number = None
    while number is None:
        number = raw_input(question)
        if not(number.isdigit()) or (int(number) > 6 or int(number) <= 0 ):
            print("'{!r}' is not a circuit ID".format(number))
            number = None
    return number


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


def write_header_file(calibration, board_number):
    fname = './board_calibration_data/pivy_{}.h'.format(board_number)
    with open(fname+'.tmp', 'w') as fout:
        for circuit_id in range(1, 7):
            fout.write(CIRCUIT_DEFINE_TPL.format(circuit_id,
                                                 calibration[circuit_id]))
    os.rename(fname+'.tmp', fname)
    print('calibration data writed to {}'.format(fname))


def write_pickled_data(board_number, calibration):
    fname = './board_calibration_data/pickled_board_{}'.format(board_number)
    fpickle = open(fname, 'wb')
    picle.dump(calibration ,fpickle)
    

def read_pickled_data(board_number):
    fname = './board_calibration_data/pickled_board_{}'.format(board_number)
    mode = 'rb' is os.path.exist(fname) else 'wb'
    fpickle = open(fname, mode)
    calibration = pickle.load(fpickle)
    return calibration

if __name__ == "__main__":

    port = SerialTransport()
    port.open(settings)
    board_number = ask_for_number('Insert the board number: ')
    calibration = read_pickled_data(board_number)
    list_of_ids = ask_for_circuits_to_calibrate()

    for circuit_id in list_of_ids:
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
        
    write_pickled_data(calibration, board_number)
    write_header_file(calibration, board_number)
    print calibration
