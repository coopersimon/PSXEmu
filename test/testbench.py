#!/usr/bin/env python

import subprocess
import sys

tests = ["test1.py"]

def main(argv):
	if len(argv) > 1:
		subprocess.Popen([argv[1]])
	else:
		for element in tests:
			subprocess.Popen([element])

if __name__ == "__main__":
	main(sys.argv)
