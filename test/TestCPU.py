#!/usr/bin/env python

import subprocess
import sys
import os

testdir = "cpu/bin/"

tests = os.listdir(testdir)

def main(argv):
    if len(argv) > 1:
        subprocess.Popen([argv[1]])
    else:
        for element in tests:
            subprocess.Popen(testdir + element)

if __name__ == "__main__":
    main(sys.argv)
