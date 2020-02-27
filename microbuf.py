#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import argparse
import os
import sys
import re
import logging
import yaml

from interface_generators import *

def main():
    parser = argparse.ArgumentParser(description='Generate interface code for various languages')

    parser.add_argument('mmsg_file', action="store", nargs="+", help="*.mmsg interface description files to use")
    parser.add_argument('--out', '-o', action='store', help='output folder to use (default: output/)',
                        default="output/")
    parser.add_argument('--verbose', '-v', action='count', help='increase verbosity level (maximum: -vv)', default=0)

    args = parser.parse_args()

    if args.verbose > 1:
        logging_level = logging.DEBUG
    elif args.verbose > 0:
        logging_level = logging.INFO
    else:
        logging_level = logging.WARNING
    logging.basicConfig(format='%(levelname)s:%(message)s', level=logging_level)

    if not os.path.exists(args.out):
        os.mkdir(args.out)
        logging.debug("Created folder {}".format(args.out))

    print("--- Generated code will be stored in {}".format(args.out))

    for mmsg_file in args.mmsg_file:
        print("--- Trying to read interface description file {}...".format(mmsg_file))

        if not mmsg_file.endswith(".mmsg"):
            logging.error("Filename {} does not end with .mmsg".format(mmsg_file))
            sys.exit(1)

        mmsg_name = os.path.splitext(os.path.basename(mmsg_file))[0]

        try:
            with open(mmsg_file, 'r') as in_stream:
                mmsg_yaml = yaml.safe_load(in_stream)
        except Exception as e:
            logging.error("Could not read file {}: {}".format(mmsg_file, e))
            sys.exit(1)

        logging.debug("Parsed YAML: {}".format(mmsg_yaml))

        if "version" not in mmsg_yaml:
            logging.error("No version specified in {}".format(mmsg_file))
            sys.exit(1)

        try:
            mmsg_version = int(mmsg_yaml["version"])
            if mmsg_yaml["version"] != mmsg_version:
                raise ValueError()
        except ValueError:
            logging.error("Version specified in {} is not an integer - skipping file")
            sys.exit(1)

        if not "content" in mmsg_yaml or mmsg_yaml["content"] is None or len(mmsg_yaml["content"]) == 0:
            logging.error("{} does not have content fields".format(mmsg_file))
            sys.exit(1)

        if not isinstance(mmsg_yaml["content"], dict):
            logging.error("{} has malformed content fields".format(mmsg_file))
            sys.exit(1)

        message = Message(mmsg_name, mmsg_version)

        for field_name, field_type in mmsg_yaml["content"].items():
            message.add_field(field_name, field_type)

        print("-- Creating MATLAB interface for message {}...".format(mmsg_name))







if __name__ == "__main__":
    main()
