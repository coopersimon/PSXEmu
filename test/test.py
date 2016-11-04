#!/usr/bin/env python

import subprocess
import sys
import os
import re

#################################

passes = {}
fails = {}

def test_pass(test, faildump):
    match = re.search('([a-zA-Z]+)_[0-9]+ (Pass|Fail)', test)
    
    if match == None:
        print("Test formatting error.")
        return

    function = match.group(1)

    if function not in passes:
        passes[function] = 0
        fails[function] = 0

    if match.group(2) == 'Pass':
        passes[function] += 1
    elif match.group(2) == 'Fail':
        fails[function] += 1
        test = test.replace('b\'', '', 1)
        test = test.replace('\'', '', 1)
        test = test.replace('\\n', '\n')
        faildump.write("\n")
        faildump.write(test)

#####################################

def print_tests():
    print("Test\t|", "Pass\t", "Fail")
    print("----------------------")
    for test in passes.keys():
        print(test, "\t|", passes[test], "\t", fails[test])

#####################################

def main(argv):
    testdir = argv[1] + '/bin/'
    tests = os.listdir(testdir)
    faildump = open(argv[1] + 'fail.output', 'w')
    for element in tests:
        out = str(subprocess.check_output(testdir + element))
        test_pass(out, faildump)
    faildump.close()
    faildump = open(argv[1] + 'fail.output', 'r')
    print_tests()
    print(faildump.read())


if __name__ == "__main__":
    main(sys.argv)
