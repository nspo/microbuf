# microbuf
Interface generator with few dependencies - e.g. for communication between ROS (C++) and a 
dSPACE MicroAutoBox

## Who might want to use `microbuf`?
You might want to use it if you plan to create simple interfaces between systems, 
possibly using network protocols like TCP or UDP. One possible example would be
"Send sensor data from computer over UDP to an embedded system". The number of supported
languages is currently pretty limited - only C++ for serializing (sender) and
MATLAB for deserializing (receiver). Due to the structure of the project, support for
more languages can be added with reasonable effort though.

The inspiration for creating `microbuf` was a computer running [ROS](https://www.ros.org/) 
which was supposed to send data to an embedded system
([dSPACE MicroAutoBox](https://www.dspace.com/en/inc/home/products/hw/micautob/microautobox2.cfm)).
The ROS sender node could use C++ to read sensors and communicate with other systems, serialize 
the data using `microbuf`, and send it via UDP on the network to the embedded system. The embedded
system could then deserialize the data (as the `MATLAB` deserializer was converted automatically 
into C before compiling for the target system) and use it for the intended purpose.

## Development state
Work in progress / some parts may not build

## Why not just use `protobuf`/`JSON`/...?
Using another library with more features like [protobuf](https://github.com/protocolbuffers/protobuf) may indeed make sense in many cases.
`microbuf` is intended for use cases with limitations, e.g. when it is hard (or even impossible) to use
external libraries. 
This may be the case when compiling for some embedded systems. 
`microbuf` does not depend on external libraries and a compilation just using standard language tools
should be possible.

## What kind of data can be sent?
The following data types are currently supported:
 - `boolean`
 - Unsigned integers: `uint8_t`, `uint16_t`, `uint32_t`, and `uint64_t`
 - Floating point: `float32_t`, `float64_t`
 - Arrays of the above with a static size
 
## What languages are supported?

| Language | Serialization | Deserialization | Notes |
|---|---|---|---|
| C++ | ✔ | ✘ | Needs `std::vector` right now |
| MATLAB | ✘ | ✔ | Usable in Simulink; compiles with Simulink/MATLAB Coder |
| ... | ✘ | ✘ | Please open a feature request or PR for new target languages |

## How is the data format specified?
`microbuf` uses message description files ending with `.mmsg` for describing the structure
of messages, not unlike ROS. 
`mmsg` files need to follow a specific structure and must be valid `YAML`. 
The following example can be saved as `SensorData.mmsg` and then be used as a `microbuf` message:

```yaml
tbd
```

## How does `microbuf` work?
The serialization and serialization which `microbuf` does is based on the 
[MessagePack](https://github.com/msgpack/msgpack/blob/master/spec.md) specification.

tbd, graphics