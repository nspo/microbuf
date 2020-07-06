#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import sys

SUPPORTED_DATATYPES = ["float32"]


class Message:
    def __init__(self, name: str, version: int):
        self.name = name
        self.version = version
        self.fields = []

    def add_field(self, field_name, field_type):
        # it makes not much sense to check whether a field with field_name already exists here as duplicate keys
        # are silently ignored by the YAML loading

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

        if not field_name[0].isalpha():
            logging.error("Field name '{}' does not begin with a letter".format(
                    field_name))
            sys.exit(1)

        self.name = field_name
        self.type = field_type


class CppEncoderGenerator:
    def __init__(self, message: Message):
        self.message = message

    def gen_header_filename(self):
        return "{}.h".format(self.message.name.lower())

    def gen_header_content(self):
        return self._gen_head()+"\n"+self._gen_foot()

    def _gen_include_guard_name(self):
        return "MICROBUF_MSG_{}_H".format(self.message.name.upper())

    def _gen_head(self):
        with open("cpp_templates/head.h", "r") as f:
            return f.read().format(include_guard_name=self._gen_include_guard_name(), message_name=self.message.name,
                                   message_version=self.message.version)

    def _gen_foot(self):
        with open("cpp_templates/foot.h", "r") as f:
            return f.read().format(include_guard_name=self._gen_include_guard_name())

