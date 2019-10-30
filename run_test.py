#!/usr/bin/python3
import sys
import subprocess
import os
from statistics import mean

RUN_COUNT = 1
TC_COUNT = 3
ARGS_DH = {
    'key_size': [64*i for i in range(1,9)]
}
ARGS_BN = {
    'tc_num': [i for i in range(3)],
    'opr':['add', 'div', 'mul', 'modexp']
}
ARGS_RSA = {
    'key_size' : [256, 512, 1024, 2048, 4096],
    'e' : [3, 17, 65537]
}
RESULT_DIR = 'result_datax'

def run_individual_test(args):
    command = ['./test.run']
    command.extend(args)
    result = []
    print('Running test:', ' '.join(args), ' RUN_COUNT ', RUN_COUNT)
    for i in range(RUN_COUNT):
        out = subprocess.run(command, stdout=subprocess.PIPE)
        result.append(float(out.stdout.decode('utf-8').rstrip()))
    print(result)
    return result

def print_file(filename, header, file_content):
    with open(filename, mode='w') as csv_file:
        writer = csv.DictWriter(csv_file, fieldnames=header)
        writer.writeheader()

        for item in file_content:
            writer.writerow(item)

def run_bntest():
    pass

def run_dhtest():
    for key_size in ARGS_DH['key_size']:
        result = run_individual_test(['dh', str(key_size)])
        pass

def run_rsatest():
    pass

def main(mode):
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
    if mode == "bn":
        run_bntest()
    elif mode == "dh":
        run_dhtest()
    elif mode == "rsa":
        run_rsatest()
    elif mode == "all":
        run_bntest()
        run_dhtest()
        run_rsatest()
    else:
        print("invalid_args")

if __name__ == '__main__':
    main(sys.argv[1])
