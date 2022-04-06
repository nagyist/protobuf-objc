// Protocol Buffers for Objective C
//
// Copyright 2010 Booyah Inc.
// Copyright 2008 Google Inc.
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

#ifndef OBJC_FILE_H__
#define OBJC_FILE_H__

#include <google/protobuf/stubs/common.h>
#include <set>
#include <string>
#include <vector>

namespace google {
namespace protobuf {
    class FileDescriptor; // descriptor.h
    namespace io {
        class Printer; // printer.h
    }
}

namespace protobuf {
    namespace compiler {
        class GeneratorContext;
        namespace objectivec {

            class FileGenerator {
            public:
                explicit FileGenerator(const FileDescriptor *file);
                ~FileGenerator();

                void GenerateSource(io::Printer *printer);
                void GenerateHeader(io::Printer *printer);
                void DetermineDependencies(set<string> *dependencies);

                void GenerateEnumsHeader(io::Printer *printer);
                void GenerateAggregateHeader(io::Printer *printer, string enumsHeaderName);
                void GenerateHeaders(GeneratorContext *outputDirectory, string extension, string aggregateHeaderName);

                const string &classname() { return classname_; }

            private:
                static std::string GetImportPrefix();

            private:
                const FileDescriptor *file_;
                string classname_;

                GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(FileGenerator);
            };
        } // namespace objectivec
    }     // namespace compiler
} // namespace protobuf
} // namespace google

#endif // OBJC_FILE_H__
