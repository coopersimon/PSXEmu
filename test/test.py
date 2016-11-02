#!/usr/bin/env python

import subprocess
import sys
import os
import re
from collections import defaultdict

#################################

passes = {}
fails = {}

def test_pass(test):
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
    if match.group(2) == 'Fail':
        fails[function] += 1

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
    for element in tests:
        out = str(subprocess.check_output(testdir + element))
        test_pass(out)
    print_tests()


if __name__ == "__main__":
    main(sys.argv)
