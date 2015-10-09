# This file is part of the Pivi firmware package.
# Released under the Creative Commons ATT-NC-ShareAlire license
# http://creativecommons.org/licenses/by-nc-sa/4.0/
# Copyright (C) 2014, 2015 LESS Industries S.A.
# Author: Diego Mascialino <dmascialino@gmail.com>

from time import sleep
import os
import struct
import pickle
import argparse
from pyvi import SerialTransport

settings = {'port': '/dev/ttyUSB0',
            'baudrate': 115200,
            'timeout': 10000}

V_RMS = 220
I_RMS = 3 


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

def send_circuits_to_calibrate(ids, port):
    while len(ids) < 6:
        ids.append(9)
    for id in ids:
        port.serial.write('{}'.format(id))
        sleep(0.01)

def write_header_file(calibration, fname):
    with open(fname+'.tmp', 'w') as fout:
        for circuit_id in range(1,7):
            fout.write(CIRCUIT_DEFINE_TPL.format(circuit_id, calibration[circuit_id]))
    os.rename(fname+'.tmp', fname)
    print('calibration data writed to {}'.format(fname))


def ask_for_number(question):
    number = None
    while number is None:
        number = raw_input(question)
        if not number.isdigit():
            print("'{!r}' is not a number".format(number))
            number = None
    return number


def write_pickled_data(calibration, pfname):
    with  open(pfname, 'wb') as fpickle:
        pickle.dump(calibration ,fpickle)
    

def read_pickled_data(pfname):
    with open(pfname, 'rb') as fpickle:  
        calibration = pickle.load(fpickle)
    return calibration


def check_file_for_pickle(fname):
    if not os.path.exists(fname):
        calibration = {} 
        for circuit_id in range(1,7):
            calibration[circuit_id]={}
            calibration[circuit_id]['v_offset'] = 0
            calibration[circuit_id]['i_offset'] = 0 
            calibration[circuit_id]['v_gain'] = 0
            calibration[circuit_id]['i_gain'] = 0 
            calibration[circuit_id]['delay'] = 0 
        write_pickled_data(calibration, fname)
    else:
        pass


if __name__ == "__main__":

    reference = 2.65 
    parser = argparse.ArgumentParser(description='Select the board and circuits to calibrate.')
    parser.add_argument('-b', dest= 'board_c', help= 'Number of board to calibrate.')
    parser.add_argument('-c', dest ='circuits', metavar='N',  type=int, nargs='+',
                       help='IDs of circuits to calibrate. (1 to 6)')
    parser.add_argument('--all', dest='board',
                       help='Calibrate all circuits in the selected board.')
    args = parser.parse_args()
    if args.circuits and args.board_c >= 1:
        for i in args.circuits:
            if not i in range(1,7):
                print 'ID of circuit out of range. (1 to 6)'
                quit()
        print 'Calibrate circuits ', args.circuits, ' in board ', args.board_c
        board_number = args.board_c
        list_of_ids = args.circuits
    elif args.board >= 1:
        print 'Calibrate all circuits in board ' , args.board
        board_number = args.board
        list_of_ids = [1 , 2, 3, 4, 5, 6]
    else:
        print 'Nothing to be done. Please use -h to see help.'  
        quit()

    port = SerialTransport()
    port.open(settings)
    pfname = './board_calibration_data/pickled_board_{}'.format(board_number)
    fname = './board_calibration_data/pivy_{}.h'.format(board_number)

    send_circuits_to_calibrate(list_of_ids, port)

    check_file_for_pickle(pfname)     
    calibration = read_pickled_data(pfname)

    for circuit_id in list_of_ids:
        if circuit_id > 0 and circuit_id < 7:
            calibration[circuit_id] = {}
            print('\n\nCalibrating circuit number  {}'.format(circuit_id))
            print('Measuring offset, disconnect V and I, and press any key')
            raw_input('')
            write_char(port)
            v_offset, i_offset = read_calibration_package(port, '<HH')
            print('v_offset = {} -> {} V , i_offset = {}-> {} V \n'.format(v_offset, v_offset*reference/4096,i_offset,i_offset*reference/4096))
            calibration[circuit_id]['v_offset'] = v_offset
            calibration[circuit_id]['i_offset'] = i_offset

            print('Measuring gain, connect V and I, and press any key')
            raw_input('')
            write_char(port)
            v_rms2, i_rms2 = read_calibration_package(port, '<II')
            print 'v_rms2 {} V , i_rms2 {} V \n'.format(v_rms2,i_rms2)
            try:
                calibration[circuit_id]['v_gain'] = V_RMS / v_rms2
                calibration[circuit_id]['i_gain'] = I_RMS / i_rms2
                print 'v_gain {} i_gain {} \n'.format(calibration[circuit_id]['v_gain'],calibration[circuit_id]['i_gain'])
            except ZeroDivisionError as e:
                print  e
                calibration[circuit_id]['v_gain'] = 1 
                calibration[circuit_id]['i_gain'] = 1
            delay = ask_for_number('Insert the delay for V channel [us]: ')
            calibration[circuit_id]['delay'] = int(float(delay) * 4) 
    print 'calibration-> ' , calibration
    write_pickled_data(calibration, pfname)
    write_header_file(calibration, fname)
