# This file is part of the Pivi firmware package.
# Released under the Creative Commons ATT-NC-ShareAlire license
# http://creativecommons.org/licenses/by-nc-sa/4.0/
# Copyright (C) 2014, 2015 LESS Industries S.A.
# Author: Diego Mascialino <dmascialino@gmail.com>

import os
import struct
import pickle
import argparse
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

CIRCUIT_DEFINE_TPL = '''

#define C{0}_V_OFFSET {1[v_offset]}
#define C{0}_I_OFFSET {1[i_offset]}
#define C{0}_V_GAIN {1[v_gain]}
#define C{0}_I_GAIN {1[i_gain]}
#define C{0}_DELAY {1[delay]}

'''


def write_header_file(calibration, board_number, list_of_ids):
    fname = './board_calibration_data/pivy_{}.h'.format(board_number)
    with open(fname+'.tmp', 'w') as fout:
        for circuit_id in list_of_ids:
            fout.write(CIRCUIT_DEFINE_TPL.format(circuit_id,
                                                 calibration[circuit_id]))
    os.rename(fname+'.tmp', fname)
    print('calibration data writed to {}'.format(fname))


def write_pickled_data(board_number, calibration):
    fname = './board_calibration_data/pickled_board_{}'.format(board_number)
    with  open(fname, 'wb') as fpickle:
        picle.dump(calibration ,fpickle)
    

def read_pickled_data(board_number):
    fname = './board_calibration_data/pickled_board_{}'.format(board_number)
    with open(fname, 'rb') as fpickle:  #if the file does not exist the program crashes
        calibration = pickle.load(fpickle)
    return calibration

def complete_the_list(list_of_ids)
    not_in_list = []
    for i in range(1,7):
        if not i in list_of_ids:
            not_in_list.append(i)
    return not_in_list


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Select the board and circuits to calibrate.')
    parser.add_argument('-b', dest= 'board_c', help= 'Number of board to calibrate.')
    parser.add_argument('-c', dest ='circuits', metavar='N',  type=int, nargs='+',
                       help='Circuits to calibate.')
    parser.add_argument('--all', dest='board',
                       help='Calibrate all circuits in the selected board.')
    args = parser.parse_args()
    if args.circuits and args.board_c >=1:
        print 'Calibrate circuits ', args.circuits, ' in board ', args.board_c
        board_number = args.board_c
        list_of_ids = args.circuits
    elif args.board >=1:
        print 'Calibrate all circuits in board ' , args.board
        board_number = args.board
        list_of_ids = [1 , 2, 3, 4, 5, 6]
    else:
        print 'Nothing to be done. Please use -h to see help.'  
        quit()
    port = SerialTransport()
    port.open(settings)
    calibration = read_pickled_data(board_number)
    #not_in_list = complete_the_list(list_of_ids)
    
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
    write_header_file(calibration, board_number, list_of_ids)
    print calibration
