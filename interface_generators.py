#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import sys

SUPPORTED_DATATYPES = ["float32"]


class Message:
    def __init__(self, name, version):
        self.name = name
        self.version = version
        self.fields = []

    def add_field(self, field_name, field_type):
        field = MessageField(field_name, field_type)
        self.fields.append(field)


class MessageField:
    def __init__(self, field_name: str, field_type: str):
        if field_type not in SUPPORTED_DATATYPES:
            logging.error("Data type {} of field {} not supported".format(field_type, field_name))
            sys.exit(1)

        if not field_name.isalnum():
            logging.error(
                "Field name '{}' is not alphanumeric, i.e. does not only contain A-Z, a-z, and 0-9".format(
                    field_name))
            sys.exit(1)

        self.name = field_name
        self.type = field_type


class CppEncoderGenerator:
    def __init__(self, message: Message):
        self.message = message

    def gen_filename(self):
        return "{}.h".format(self.message.name)
