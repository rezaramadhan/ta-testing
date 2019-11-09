#!/usr/bin/python3
import sys
import subprocess
import os
import itertools
import csv
import re
from copy import deepcopy
from statistics import mean
from pprint import pprint
from shutil import copyfile

MAKE_GRAPH = False

COPY_RESULT = False
TEX_DIR = '../../ta-latex/src/resources/csv/'

RUN_COUNT = 5
DATA_SIZE = 100

DH_START = 20
DH_INCREMENT = 4

RSA_KSTART = 32
RSA_KINCREMENT = 16
RSA_KDATA_DIFF = 0

RSA_ESTART = 29
RSA_EINCREMENT = 32

DEVICE_ID = 1

ARGS = {
    'dh': {
        '1.key_size': [DH_INCREMENT * i for i in range(DH_START, DATA_SIZE + DH_START)]
    },
    'bn': {
        '1.tc_num': {
            'add' : [128*16*i*8 for i in range(1, DATA_SIZE + 1)],
            'mul' : [128*8*i*8 for i in range(1, DATA_SIZE + 1)],
            'div' : [128*8*i*8 for i in range(1, DATA_SIZE + 1)],
            'modexp' : [128*8*i*8 for i in range(1, DATA_SIZE + 1)],
            'modmul' : [32*i*8 for i in range(1, DATA_SIZE + 1)],
        },
        # '2.opr': ['add', 'mul', 'div', 'modexp', 'modmul']
        '2.opr': ['add']
    },
    'rsa_gen': {
        '1.key_size': [RSA_KINCREMENT * i for i in range(RSA_KSTART, DATA_SIZE + RSA_KSTART - RSA_KDATA_DIFF)],
        '2.e': [65537],
    },
    'rsa_enc': {
        '1.msg_size': [RSA_EINCREMENT * i for i in range(RSA_ESTART, DATA_SIZE + RSA_ESTART)],
    }
}

RESULT_DIR = 'result_data/'

def run_individual_test(args):
    command = ['./test.run']
    command.extend(args)
    result = []
    print('Running test (' + str(RUN_COUNT) + ' times):', ' '.join(command))
    for i in range(RUN_COUNT):
        out = subprocess.run(command, stdout=subprocess.PIPE)
        # print(out.stdout)
        result.append(float(out.stdout.decode('utf-8').rstrip()))
        # result.append(i)
    return result

def print_file_csv(filename, header, file_content):
    with open(filename, mode='w') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=header)
        writer.writeheader()

        for item in file_content:
            writer.writerow(item)
    #
    with open(filename, mode='r') as fin:
        lines = fin.readlines()

    lines[0] = re.sub(r'\d+\.','',lines[0])

    with open(filename, mode='w') as fout:
        fout.writelines(lines)

    print('-------------------------------')
    print('Writing result to', filename)
    print('-------------------------------')

    copy_result(filename)

def copy_result(filename):
    if (COPY_RESULT):
        print('File', filename, 'copied to', TEX_DIR)
        fname = filename.split('/')[1]
        copyfile(filename, TEX_DIR + fname)

def separate_csv(filename):
    with open(filename, mode='r') as fin:
        rows = fin.readlines()

        outfiles = {}
        for opr in ARGS['bn']['2.opr']:
            sep_fname = filename.replace('.', '_' + opr + '.')
            of = open(sep_fname, mode='w')
            of.write(rows[0])
            outfiles[opr] = of

        for row in rows[1:]:
            col = row.rstrip().split(',')
            # print(col)
            outfiles[col[-1]].write(row)

        # Close files
        for of in outfiles.values():
            fname = of.name
            of.close()
            copy_result(fname)


def get_params(mode):
    command_args = []
    if (mode != 'bn'):
        for var1 in sorted(ARGS[mode].keys()):
            var_list = ARGS[mode][var1]
            # print(var_list)
            if len(command_args) == 0: # list empty
                command_args = [{var1: str(i)} for i in var_list]
            else:
                c = []
                for y in var_list:
                    for x in command_args:
                        z = deepcopy(x)
                        z[var1] = str(y)
                        c.append(z)
                # print(c)
                command_args = c
    else: # mode == 'bn'
        for opr in ARGS['bn']['2.opr']:
            for tc in ARGS['bn']['1.tc_num'][opr]:
                command_args.append({
                    '2.opr': opr,
                    '1.tc_num': str(tc),
                })

    return command_args

def main(arg):
    # make file
    print("-------------------------------")
    print("Building tests")
    print("-------------------------------\n")

    subprocess.run(['make'])
    if not os.path.exists(RESULT_DIR):
        os.makedirs(RESULT_DIR)

    print("\n-------------------------------")
    print("Build done")
    print("-------------------------------\n\n")

    # check arg
    try:
        test_modes = []
        if (arg == "all"):
            test_modes = list(ARGS.keys())
        else:
            test_modes = [arg]
        # print(test_modes)


        for mode in test_modes:
            command_args = get_params(mode)
            # pprint(command_args)
            csv_headers = sorted(ARGS[mode].keys())

            test_data = []
            i = 0
            for arg in command_args:
                header = sorted(arg.keys())
                a = [arg[k] for k in header]
                # print(arg)
                command = [mode]
                command.extend(a)
                result = run_individual_test(command)
                print(i, result, '\n')
                i = i + 1
                row = arg.copy()
                row_avg = arg.copy()
                header.extend(['0.time', '0.time_avg'])
                row['0.time'] = str(result).replace(', ', '|')
                row['0.time_avg'] = round(mean(result), 2)
                test_data.append(row)

            # pprint(test_data)
            filename = RESULT_DIR + mode + str(DEVICE_ID) + '.csv'
            print_file_csv(filename, sorted(header), test_data)

            if mode == 'bn':
                separate_csv(filename)

        if MAKE_GRAPH:
            cmd = 'latexmk -pdf -outdir=result_data -cd result_data/graph.tex'
            subprocess.run(cmd.split(' '), stdout=subprocess.PIPE)

    except KeyError as e:
        print("invalid_args")

if __name__ == '__main__':
    if (len(sys.argv) == 2):
        main(sys.argv[1])
    else:
        print("invalid_args")
