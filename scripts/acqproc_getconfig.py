#!/usr/bin/env python

"""
Script parses ACQPROC config file to create environment variables UUT1 [UUT2...] UUTS

Usage:


"""


import argparse
from os.path import expanduser
import os
import json


def parse_config(path):
    print("# parse_config {}".format(path))
    with open(path) as f:
        jdata = json.load(f)

    uuts=''
    ii = 1

    for uut in jdata['AFHBA']['UUT']:
        name = uut['name']
        print("UUT{}={}".format(ii, uut['name']))
        uuts = '{}{} '.format(uuts, uut['name'])
        ii += 1

    print('UUTS="{}"'.format(uuts))
    print('DEVMAX={}'.format(ii))


def run_main():
    parser = argparse.ArgumentParser(description='acqproc_getconfig')
    parser.add_argument('config', help="path to config file")
    args = parser.parse_args();
    parse_config(args.config)


if __name__ == '__main__':
    run_main()
