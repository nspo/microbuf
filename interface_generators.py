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
        uint8: 1 + 1,
        uint16: 1 + 2,
        uint32: 1 + 4,
        uint64: 1 + 8,
        float32: 1 + 4,
        float64: 1 + 8
    }

    all = (bool, uint8, uint16, uint32, uint64, float32, float64)  # simplify this?


class ArrayTypes:
    fixarray = "fixarray"
    array16 = "array16"
    array32 = "array32"

    storage_size = {
        fixarray: 1,
        array16: 1 + 2,
        array32: 1 + 4
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

        if field_name == "err":
            logging.error("The field name 'err' is forbidden")
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
        return self.array_length * 1

    def get_num_of_bytes(self):
        # does not include actually marking this as its own array
        return self.array_length * PlainTypes.storage_size[self.type]


class Message:
    def __init__(self, name: str, version: int, append_checksum: bool):
        self.name = name
        self.version = version
        self.append_checksum = append_checksum
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
            num = num + field.get_num_of_plain_fields()

        # do not count checksum field at the end b/c it's not part of the main array

        return num

    def get_main_array_type(self):
        num_plain_fields = self.get_num_of_plain_fields()
        if num_plain_fields <= ArrayTypes.max_length[ArrayTypes.fixarray]:
            return ArrayTypes.fixarray
        elif num_plain_fields <= ArrayTypes.max_length[ArrayTypes.array16]:
            return ArrayTypes.array16
        elif num_plain_fields <= ArrayTypes.max_length[ArrayTypes.array32]:
            return ArrayTypes.array32
        else:
            logging.error("Too many plain fields ({}, excluding initial array) in message '{}'".format(num_plain_fields,
                                                                                                       self.name))
            sys.exit(1)

    def get_num_of_bytes(self):
        """ Calculate number of bytes needed to store this message (excluding a possible CRC value)
        """
        num_bytes = 0
        for field in self.fields:
            num_bytes = num_bytes + field.get_num_of_bytes()

        # add size of leading array
        num_bytes = num_bytes + ArrayTypes.storage_size[self.get_main_array_type()]

        if self.append_checksum:
            # also count checksum field at the end b/c it needs storage space
            num_bytes = num_bytes + PlainTypes.storage_size[PlainTypes.uint16]

        return num_bytes


class CppInterfaceGenerator:
    DATA_TYPE_LOOKUP = {
        # microbuf data type: [C++ data type, C++ serialization method]
        PlainTypes.bool: ["bool", "gen_bool", "parse_bool"],
        PlainTypes.uint8: ["uint8_t", "gen_uint8", "parse_uint8"],
        PlainTypes.uint16: ["uint16_t", "gen_uint16", "parse_uint16"],
        PlainTypes.uint32: ["uint32_t", "gen_uint32", "parse_uint32"],
        PlainTypes.uint64: ["uint64_t", "gen_uint64", "parse_uint64"],
        PlainTypes.float32: ["float", "gen_float32", "parse_float32"],
        PlainTypes.float64: ["double", "gen_float64", "parse_float64"]
    }

    ARRAY_GEN_LOOKUP = {
        ArrayTypes.fixarray: "gen_fixarray",
        ArrayTypes.array16: "gen_array16",
        ArrayTypes.array32: "gen_array32"
    }

    ARRAY_CHECK_LOOKUP = {
        ArrayTypes.fixarray: "check_fixarray",
        ArrayTypes.array16: "check_array16",
        ArrayTypes.array32: "check_array32"
    }

    def __init__(self, message: Message):
        self.message = message

    def gen_header_filename(self):
        return "{}.h".format(self.message.name)

    def gen_header_content(self):
        result = []  # type: typing.List[str]
        self._gen_head(result)
        result.append("\n")
        self._gen_struct(result)
        result.append("\n")
        self._gen_foot(result)
        return "".join(result)

    @staticmethod
    def _get_plain_cpp_data_type(field_type: str):
        """ Get the correct C++ data type for a microbuf plain data type """
        if field_type not in CppInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("The C++ type for '{}' is unknown".format(field_type))
            sys.exit(1)

        return CppInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][0]

    def _gen_include_guard_name(self):
        return "MICROBUF_MSG_{}_H".format(self.message.name.upper())

    def _gen_head(self, result):
        s = ("#ifndef {include_guard_name}\n"
             "#define {include_guard_name}\n"
             "// Microbuf Message: {message_name}\n"
             "// Version {message_version}\n"
             '#include "microbuf.h"\n'
             )

        result.append(s.format(include_guard_name=self._gen_include_guard_name(), message_name=self.message.name,
                               message_version=self.message.version))

    def _gen_foot(self, result):
        result.append("#endif // {include_guard_name}\n".format(include_guard_name=self._gen_include_guard_name()))

    def _get_definition_line(self, field: MessageField):
        """ Generate C++ definition line of a field """
        if type(field) == MessageFieldPlain:  # check without inheritance here (could e.g. be array otherwise)
            field = typing.cast(MessageFieldPlain, field)
            return "{} {};".format(self._get_plain_cpp_data_type(field.type), field.name)
        elif type(field) == MessageFieldPlainArray:
            field = typing.cast(MessageFieldPlainArray, field)
            return "{} {}[{}];".format(self._get_plain_cpp_data_type(field.type), field.name, field.array_length)

        logging.error("No definition for field {}".format(field.name))
        sys.exit(1)

    @staticmethod
    def _get_serialization_line_plain(field_type: str, object_name: str, byte_index: int):
        """ Get the C++ serialization line of a plain field """
        if field_type not in CppInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("Serialization for type {} is unknown".format(field_type))
            sys.exit(1)

        return "microbuf::insert_bytes<{}>(bytes, microbuf::{}({}));".format(
            byte_index, CppInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][1], object_name
        )

    @staticmethod
    def _get_serialization_line_plainarray(field_type: str, object_name: str, byte_index: int, num_elements: int):
        """ Get the C++ serialization line of a plain array field """
        if field_type not in CppInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("Serialization for type {} is unknown".format(field_type))
            sys.exit(1)

        return "microbuf::insert_bytes<{}>(bytes, microbuf::gen_multiple<{}>({}, microbuf::{}));".format(
            byte_index, num_elements, object_name, CppInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][1]
        )

    def _gen_array_serialization_line(self):
        return "microbuf::insert_bytes<0>(bytes, microbuf::{}({}));\n".format(
            CppInterfaceGenerator.ARRAY_GEN_LOOKUP[self.message.get_main_array_type()],
            self.message.get_num_of_plain_fields())

    @staticmethod
    def _gen_return_on_error():
        return "if(!worked) { return false; }"

    def _gen_struct(self, result):
        s4 = "    "  # spaces
        result.append("struct {}_struct_t {{\n".format(self.message.name))
        result.append(
            "".join([s4, "static constexpr size_t data_size = {};\n\n".format(self.message.get_num_of_bytes())]))
        for field in self.message.fields:
            result.append("".join([s4, self._get_definition_line(field), "\n"]))

        # add as_bytes() for serialization
        result.append("".join([s4, "\n"]))
        result.append(
            "".join([s4, "microbuf::array<uint8_t,{}> as_bytes() const {{\n".format(self.message.get_num_of_bytes())]))
        result.append(
            "".join([s4 * 2, "microbuf::array<uint8_t,{}> bytes {{}};\n".format(self.message.get_num_of_bytes())]))
        result.append("".join([s4 * 2, self._gen_array_serialization_line()]))
        byte_index = 0 + ArrayTypes.storage_size[self.message.get_main_array_type()]
        for field in self.message.fields:
            if type(field) == MessageFieldPlain:
                field = typing.cast(MessageFieldPlain, field)
                result.append(
                    "".join([s4 * 2, self._get_serialization_line_plain(field.type, field.name, byte_index) + "\n"]))
                byte_index = byte_index + PlainTypes.storage_size[field.type]
            elif type(field) == MessageFieldPlainArray:
                field = typing.cast(MessageFieldPlainArray, field)
                result.append("".join([s4 * 2,
                                       self._get_serialization_line_plainarray(field.type, field.name, byte_index,
                                                                               field.array_length) + "\n"]))
                byte_index = byte_index + PlainTypes.storage_size[field.type] * field.array_length
            else:
                logging.error("Unknown field object {}".format(field))
                sys.exit(1)

        if self.message.append_checksum:
            result.append("".join([s4 * 2, "microbuf::append_crc(bytes);\n"]))

        result.append("".join([s4 * 2, "return bytes;\n"]))
        result.append("".join([s4, "}\n"]))

        # add from_bytes() for deserialization
        result.append("".join([s4, "\n"]))
        result.append("".join([s4, "bool from_bytes(const microbuf::array<uint8_t,{}>& bytes) {{\n".format(
            self.message.get_num_of_bytes())]))
        result.append("".join([s4 * 2, "bool worked = microbuf::{}<0>(bytes, {});\n".format(
            CppInterfaceGenerator.ARRAY_CHECK_LOOKUP[self.message.get_main_array_type()],
            self.message.get_num_of_plain_fields())]))
        result.append("".join([s4 * 2, self._gen_return_on_error(), "\n"]))

        byte_index = 0 + ArrayTypes.storage_size[self.message.get_main_array_type()]
        for field in self.message.fields:
            if type(field) == MessageFieldPlain:
                field = typing.cast(MessageFieldPlain, field)
                result.append("".join([s4 * 2, "worked = microbuf::{}<{}>(bytes, {});\n".format(
                    CppInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][2], byte_index, field.name)]))
                result.append("".join([s4 * 2, self._gen_return_on_error(), "\n"]))
                byte_index = byte_index + PlainTypes.storage_size[field.type]
            elif type(field) == MessageFieldPlainArray:
                field = typing.cast(MessageFieldPlainArray, field)
                result.append(
                    "".join([s4 * 2, "worked = microbuf::parse_multiple<{},{}>(bytes, {}, microbuf::{}<0>);\n".format(
                        field.array_length, byte_index, field.name,
                        CppInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][2])]))
                result.append("".join([s4 * 2, self._gen_return_on_error(), "\n"]))
                byte_index = byte_index + PlainTypes.storage_size[field.type] * field.array_length
            else:
                logging.error("Unknown field object {}".format(field))
                sys.exit(1)

        if self.message.append_checksum:
            result.append("".join([s4 * 2, "worked = microbuf::verify_crc(bytes);\n"]))

        result.append("".join([s4 * 2, "return worked;\n"]))
        result.append("".join([s4, "}\n"]))

        result.append("".join(["};\n\n"]))


class MatlabInterfaceGenerator:
    DATA_TYPE_LOOKUP = {
        # microbuf data type: [MATLAB data type, MATLAB init value, MATLAB deserialize function]
        PlainTypes.bool: ["bool", "false", "microbuf.parse_bool"],
        PlainTypes.uint8: ["uint8", "0", "microbuf.parse_uint8"],
        PlainTypes.uint16: ["uint16", "0", "microbuf.parse_uint16"],
        PlainTypes.uint32: ["uint32", "0", "microbuf.parse_uint32"],
        PlainTypes.uint64: ["uint64", "0", "microbuf.parse_uint64"],
        PlainTypes.float32: ["single", "0", "microbuf.parse_float32"],
        PlainTypes.float64: ["double", "0", "microbuf.parse_float64"],
    }

    ARRAY_CHECK_LOOKUP = {
        ArrayTypes.fixarray: "check_fixarray",
        ArrayTypes.array16: "check_array16",
        ArrayTypes.array32: "check_array32"
    }

    def __init__(self, message: Message):
        self.message = message

    def gen_deserializer_filename(self):
        return "deserialize_{}.m".format(self.message.name)

    @staticmethod
    def _get_initialization_line(field: MessageField):
        if type(field) == MessageFieldPlain or type(field) == MessageFieldPlainArray:
            field = typing.cast(typing.Union[MessageFieldPlain, MessageFieldPlainArray], field)
            if field.type not in MatlabInterfaceGenerator.DATA_TYPE_LOOKUP:
                logging.error("Initialization for type {} is unknown".format(field.type))
                sys.exit(1)

        if type(field) == MessageFieldPlain:
            return "{} = {}({});\n".format(field.name, MatlabInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][0],
                                           MatlabInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][1])
        elif type(field) == MessageFieldPlainArray:
            return "{} = repmat({}({}), 1, {});\n".format(field.name,
                                                          MatlabInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][0],
                                                          MatlabInterfaceGenerator.DATA_TYPE_LOOKUP[field.type][1],
                                                          field.array_length)

    @staticmethod
    def _get_plain_deserialization_lines(field_type: str, object_name, line_indent: str = ""):
        """ Get the C++ serialization line of a plain field """
        if field_type not in MatlabInterfaceGenerator.DATA_TYPE_LOOKUP:
            logging.error("Deserialization for type {} is unknown".format(field_type))
            sys.exit(1)

        lines = []
        lines.append("".join([line_indent, "[idx, err, {}] = {}(bytes, bytes_length, idx);\n".format(
                                object_name, MatlabInterfaceGenerator.DATA_TYPE_LOOKUP[field_type][2])]))
        lines.append("".join([line_indent, "if err ;return; end\n"]))

        return "".join(lines)

    def _get_deserialization_lines(self, field: MessageField):
        lines = []
        if type(field) == MessageFieldPlain:
            field = typing.cast(MessageFieldPlain, field)
            lines.append("".join([self._get_plain_deserialization_lines(field.type, field.name)]))
        elif type(field) == MessageFieldPlainArray:
            field = typing.cast(MessageFieldPlainArray, field)
            lines.append("".join(["for i=1:{}\n".format(field.array_length)]))
            lines.append("".join([self._get_plain_deserialization_lines(field.type, "{}(i)".format(field.name),
                                                                        "    ")]))
            lines.append("".join(["end\n\n"]))
        else:
            logging.error("Field type unknown: {}".format(field))
            sys.exit(1)

        return "".join(lines)

    def gen_deserializer_content(self):
        result = []  # type: typing.List[str]

        result.append("function [err, {}] = deserialize_{}(bytes, bytes_length)".format(
            ", ".join(f.name for f in self.message.fields), self.message.name))
        result.append("\n")
        result.append("% deserialize_{} Parse microbuf message {} (version {})\n\n".format(
                                self.message.name, self.message.name, self.message.version))
        result.append("err = true;\n\n")

        for field in self.message.fields:
            result.append(self._get_initialization_line(field))

        result.append("\n")

        # check length of bytes array
        result.append("if bytes_length < {}\n    return\nend\n\n".format(self.message.get_num_of_bytes()))

        result.append("idx = 1;\n\n")

        # check initial array
        result.append("[idx, err] = microbuf.{}(bytes, bytes_length, {}, idx);\n".format(
                        MatlabInterfaceGenerator.ARRAY_CHECK_LOOKUP[self.message.get_main_array_type()],  # unchecked
                        self.message.get_num_of_plain_fields()))
        result.append("if err ;return; end\n\n")

        for field in self.message.fields:
            result.append(self._get_deserialization_lines(field))

        if self.message.append_checksum:
            result.append("[err] = microbuf.check_crc(bytes, bytes_length, idx);\n\n")
            # no "if err; return; end" needed here b/c we're at the end of the function already

        result.append("end")
        return "".join(result)
