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

RUN_COUNT = 5
TC_COUNT = 20
ARGS = {
    'dh': {
        '1.key_size': [64 * i for i in range(1, 19)]
    },
    'bn': {
        '1.tc_num': [8192*i*8 for i in range(1, TC_COUNT + 1)],
        # '2.opr': ['add', 'div', 'mul', 'modexp']
        '2.opr': ['add']
    },
    'rsa': {
        '1.key_size': [64 * i for i in range(1, 5)],
        '2.e': [3, 17, 65537],
        # '3.msg_size': [10, 100, 1000],
    }
}

RESULT_DIR = 'result_data/'

def run_individual_test(args):
    command = ['./test.run']
    command.extend(args)
    result = []
    print('Running test:', ' '.join(command))
    print('  RUN_COUNT ', str(RUN_COUNT))
    for i in range(RUN_COUNT):
        out = subprocess.run(command, stdout=subprocess.PIPE)
        # print(out.stdout)
        result.append(float(out.stdout.decode('utf-8').rstrip()))
    return result

def print_file_csv(filename, header, file_content):
    with open(filename, mode='w') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=header)
        writer.writeheader()

        for item in file_content:
            print(item)
            writer.writerow(item)

    with open(filename, mode='r') as fin:
        lines = fin.readlines()

    lines[0] = re.sub(r'\d+\.','',lines[0])

    with open(filename, mode='w') as fout:
        fout.writelines(lines)

def get_params(mode):
    command_args = []
    # var_lists =
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
            for arg in command_args:
                header = sorted(arg.keys())
                a = [arg[k] for k in header]
                print(arg)
                command = [mode]
                command.extend(a)
                result = run_individual_test(command)

                print(result, "\n")
                row = arg.copy()
                row_avg = arg.copy()
                header.extend(['0.time', '0.time_avg'])
                row['0.time'] = str(result).replace(', ', '|')
                row['0.time_avg'] = round(mean(result), 2)
                test_data.append(row)

            pprint(test_data)
            filename = RESULT_DIR + mode + '.csv'
            print_file_csv(filename, sorted(header), test_data)

    except KeyError as e:
        print("invalid_args")

if __name__ == '__main__':
    if (len(sys.argv) == 2):
        main(sys.argv[1])
    else:
        print("invalid_args")
