// Protocol Buffers for Objective C
//
// Copyright 2010 Booyah Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "objc_generator.h"
#include <google/protobuf/compiler/plugin.h>
#include <iostream>

using namespace google::protobuf::compiler::objectivec;

int main(int argc, char **argv) {
    if(argc == 2 && strcmp(argv[1], "-version") == 0) {
        std::cout << "1.1.5" << std::endl;
        exit(0);
    }

    ObjectiveCGenerator generator;
    return PluginMain(argc, argv, &generator);
}
