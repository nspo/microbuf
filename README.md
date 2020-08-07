# microbuf
Data serialization toolchain for C++ and embedded systems with MATLAB support

## Who might want to use `microbuf`?
You might want to use it if you plan to create an interface between a computer and an embedded system with MATLAB support, possibly using network protocols like TCP or UDP. 
One possible use case would be "Send control data from computer over UDP to embedded system". 
The number of supported languages is currently pretty limited - only C++ for serializing (Tx) and MATLAB for deserializing (Rx). 
This makes it possible to e.g. send data from a [ROS](https://www.ros.org/) node to a [dSPACE MicroAutoBox](https://www.dspace.com/en/inc/home/products/hw/micautob/microautobox2.cfm).
Due to the structure of the project, support for more languages can be added with reasonable effort.

## Why not just use `protobuf`/`JSON`/...?
Using another framework with more features like [protobuf](https://github.com/protocolbuffers/protobuf) may indeed make sense in many cases.
`microbuf` is intended for use cases with strong limitations, e.g. embedded systems.
`microbuf` does not depend on external libraries and a compilation just using standard language tools
is possible.

## What kind of data can be sent?
The following data types are currently supported:
 - `boolean`
 - Unsigned integers: `uint8_t`, `uint16_t`, `uint32_t`, and `uint64_t`
 - Floating point: `float32_t`, `float64_t`
 - Arrays of the above with a static size
 
## What languages are supported?

| Language | Serialization | Deserialization | CRC support | Notes |
|---|---|---|---|---|
| C++ | ✔ | ✘ | ✔ | Needs `std::vector` right now |
| MATLAB | ✘ | ✔ | ✔ |Usable in Simulink; compiles with Simulink/MATLAB Coder |
| ... | ✘ | ✘ | ✘ | Please open a feature request or PR for new target languages |

## How does it work?
`microbuf` uses message description files ending with `.mmsg` for describing the structure
of messages, not unlike ROS. 
`mmsg` files need to follow a specific structure and must be valid `YAML`. 
The following example can be saved as `SensorData.mmsg` and then be used as a `microbuf` message:

```yaml
version: 1
append_checksum: yes
content:
  distance: float32[10]
  angle: float32[10]
  robot_id: uint8
```

When you now execute `./microbuf.py SensorData.mmsg`, serializers and deserializers for the supported languages will automatically be generated.
You can use the serializers to convert data to bytes, send them to your receiver, and decode them there with the deserializers.


## How is data serialized?
The serialization which `microbuf` does is based on the 
[MessagePack](https://github.com/msgpack/msgpack/blob/master/spec.md) specification.
All data elements are packed into a flat array and an optional CRC16 checksum is appended.

## Example: Computer to MicroAutoBox
Suppose you want to send the message `SensorData.mmsg` mentioned above from a computer (using C++, e.g. a ROS node) to a dSPACE MicroAutoBox.
`microbuf` will generate a header file `SensorData.h` which you can include in your C++ code.
This message header file needs access to the `microbuf.h` header file, so just copy both files to the same folder where your compiler will find them.
You can then use the struct `SensorData_struct_t` in C++ to fill in your data and convert them to a byte vector.

(to be continued)