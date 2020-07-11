#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import logging
import sys
import typing
from abc import ABC, abstractmethod
import string


class PlainTypes:
    bool = "bool"
    uint8 = "uint8"
    uint16 = "uint16"
    uint32 = "uint32"
    uint64 = "uint64"
    float32 = "float32"
    float64 = "float64"

    storage_size = {
        # storage size in bytes needed for each plain field
        bool: 1,
        uint8: 1+1,
        uint16: 1+2,
        uint32: 1+4,
        uint64: 1+8,
        float32: 1+4,
        float64: 1+8
    }

    all = (bool, uint8, uint16, uint32, uint64, float32, float64)  # simplify this?

class ArrayTypes:
    fixarray = "fixarray"
    array16 = "array16"
    array32 = "array32"

    storage_size = {
        fixarray: 1,
        array16: 1+2,
        array32: 1+4
    }

    max_length = {
        fixarray: 15,
        array16: 65535,
        array32: 4294967295
    }


class MessageField(ABC):
    """ Abstract class: field inside a message"""

    @abstractmethod
    def __init__(self, field_name: str):
        # check field name
        allowed_chars = set(string.ascii_lowercase + string.ascii_uppercase + string.digits + '_')
        if set(field_name) > allowed_chars:
            logging.error("Field name '{}' does not only contain letters, numbers or underscores".format(field_name))
            sys.exit(1)

        if not field_name[0].isalpha():
            logging.error("Field name '{}' does not begin with a letter".format(
                    field_name))
            sys.exit(1)

        self.name = field_name

    @abstractmethod
    def get_num_of_plain_fields(self):
        """ Return number of plain/flat fields """
        pass

    @abstractmethod
    def get_num_of_bytes(self):
        """ Return number of bytes needed for storing this field in the serialized format """
        pass


class MessageFieldPlain(MessageField):
    """ Field inside a message with a plain data type (e.g. float32) """
    def __init__(self, field_name: str, field_type: str):
        super().__init__(field_name)

        if field_type not in PlainTypes.all:
            logging.error("Field type '{}' of field '{}' is unknown".format(field_type, field_name))
            sys.exit(1)

        self.type = field_type

    def get_num_of_plain_fields(self):
        return 1

    def get_num_of_bytes(self):
        return PlainTypes.storage_size[self.type]


class MessageFieldPlainArray(MessageFieldPlain):
    """ Field inside a message which contains an array of plain data types (e.g. float32[4]) """
    def __init__(self, field_name: str, field_type: str, array_length: int):
        super().__init__(field_name, field_type)

        if array_length < 1:
            logging.error("Field '{}' must have a length of at least 1 (has: {})".format(field_name, array_length))
            sys.exit(1)

        self.array_length = array_length

    def get_num_of_plain_fields(self):
        return self.array_length*1

    def get_num_of_bytes(self):
        # does not include actually marking this as its own array
        return self.array_length*PlainTypes.storage_size[self.type]


class Message:
    def __init__(self, name: str, version: int):
        self.name = name
        self.version = version
        self.fields = []  # type: typing.List[MessageField]

    def add_field(self, field: MessageField):
        # it makes not much sense to check whether a field with field_name already exists here as duplicate keys
        # are silently ignored by the YAML loading

        self.fields.append(field)

    def get_num_of_plain_fields(self):
        """ Calculate number of plain/flat fields
        """
        num = 0
        for field in self.fields:
            num = num+field.get_num_of_plain_fields()

        return num

    def get_num_of_bytes(self):
        """ Calculate number of bytes needed to store this message (excluding a possible CRC value)
        """
        num_bytes = 0
        for field in self.fields:
            num_bytes = num_bytes + field.get_num_of_bytes()

        # add size of leading array
        num_plain_fields = self.get_num_of_plain_fields()
        if num_plain_fields <= ArrayTypes.max_length[ArrayTypes.fixarray]:
            num_bytes = num_bytes + ArrayTypes.storage_size[ArrayTypes.fixarray]
        elif num_plain_fields <= ArrayTypes.max_length[ArrayTypes.array16]:
            num_bytes = num_bytes + ArrayTypes.storage_size[ArrayTypes.array16]
        elif num_plain_fields <= ArrayTypes.max_length[ArrayTypes.array32]:
            num_bytes = num_bytes + ArrayTypes.storage_size[ArrayTypes.array32]
        else:
            logging.error("Too many plain fields ({}, excluding initial array) in message '{}'".format(num_plain_fields,
                                                                                                       self.name))
            sys.exit(1)

        return num_bytes


class CppInterfaceGenerator:
    DATA_TYPE_LOOKUP = {
        # microbuf data type: [C++ data type, C++ serialization method]
        PlainTypes.bool: ["bool", "append_bool"],
        PlainTypes.uint8: ["uint8_t", "append_uint8"],
        PlainTypes.uint16: ["uint16_t", "append_uint16"],
        PlainTypes.uint32: ["uint32_t", "append_uint32"],
        PlainTypes.uint64: ["uint64_t", "append_uint64"],
        PlainTypes.float32: ["float", "append_float32"],
        PlainTypes.float64: ["double", "append_float64"]
    }

    def __init__(self, message: Message):
        self.message = message

    def gen_header_filename(self):
        return "{}.h".format(self.message.name)

    def gen_header_content(self):
        return self._gen_head()+"\n\n"+self._gen_struct()+"\n\n"+self._gen_foot()

    def _get_plain_cpp_data_type(self, field_type: str):
        """ Get the correct C++ data type for a microbuf plain data type """
        if field_type not in CppInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("The C++ type for '{}' is unknown".format(field_type))
            sys.exit(1)

        return CppInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][0]

    def _gen_include_guard_name(self):
        return "MICROBUF_MSG_{}_H".format(self.message.name.upper())

    def _gen_head(self):
        with open("cpp_templates/head.h", "r") as f:
            return f.read().format(include_guard_name=self._gen_include_guard_name(), message_name=self.message.name,
                                   message_version=self.message.version)

    def _gen_foot(self):
        with open("cpp_templates/foot.h", "r") as f:
            return f.read().format(include_guard_name=self._gen_include_guard_name())

    def _get_definition_line(self, field: MessageField):
        """ Generate C++ definition line of a field """
        if type(field) == MessageFieldPlain:  # check without inheritance here (could e.g. be array otherwise)
            return "{} {};".format(self._get_plain_cpp_data_type(field.type), field.name)
        elif type(field) == MessageFieldPlainArray:
            return "{} {}[{}];".format(self._get_plain_cpp_data_type(field.type), field.name, field.array_length)

        logging.error("No definition for field {}".format(field.name))
        sys.exit(1)

    def _get_serialization_line(self, field_type: str, object_name: str):
        """ Get the C++ serialization line of a plain field """
        if field_type not in CppInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("Serialization for type {} is unknown".format(field_type))
            sys.exit(1)

        return "microbuf::{}(bytes, {});".format(CppInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][1], object_name)

    def _gen_struct(self):
        S4 = "    " # spaces
        str_struct = "struct {}_struct_t {{\n".format(self.message.name)
        for field in self.message.fields:
            str_struct += S4 + self._get_definition_line(field) + "\n"

        # add as_bytes() for serialization
        str_struct += S4+"\n"
        str_struct += S4+"std::vector<uint8_t> as_bytes() const {\n"
        str_struct += S4*2+"std::vector<uint8_t> bytes;\n"
        str_struct += S4*2+"bytes.reserve({});\n".format(self.message.get_num_of_bytes())
        str_struct += S4*2+"microbuf::append_array(bytes, {});\n".format(self.message.get_num_of_plain_fields())
        for field in self.message.fields:
            if type(field) == MessageFieldPlain:
                str_struct += S4*2+self._get_serialization_line(field.type, field.name)+"\n"
            elif type(field) == MessageFieldPlainArray:
                str_struct += S4*2+"for(size_t i=0; i<{}; ++i) {{\n".format(field.array_length)
                str_struct += S4*3+self._get_serialization_line(field.type, field.name+"[i]")+"\n"
                str_struct += S4*2+"}\n"
            else:
                logging.error("Unknown field object {}".format(field))
                sys.exit(1)

        str_struct += S4*2+"return bytes;\n"
        str_struct += S4+"}\n"

        str_struct += "};\n"

        return str_struct


