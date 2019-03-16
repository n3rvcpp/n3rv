#!/bin/bash

## Compiles Protobuf files for n3rv.
cd src/protocol/protobuf
protoc n3rvproto.proto --cpp_out=.
