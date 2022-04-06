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

#include "objc_message_field.h"

#include <map>
#include <string>

#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/wire_format.h>

#include "objc_helpers.h"

namespace google {
namespace protobuf {
    namespace compiler {
        namespace objectivec {

            namespace {
                void SetMessageVariables(const FieldDescriptor *descriptor,
                    map<string, string> *variables) {
                    std::string name          = UnderscoresToCamelCase(descriptor);
                    (*variables)["classname"] = ClassName(descriptor->containing_type());
                    if(IsReservedName(name)) {
                        (*variables)["name"]             = name + "Property";
                        (*variables)["capitalized_name"] = UnderscoresToCapitalizedCamelCase(descriptor) + "Property";
                    } else {
                        (*variables)["name"]             = name;
                        (*variables)["capitalized_name"] = UnderscoresToCapitalizedCamelCase(descriptor);
                    }

                    (*variables)["list_name"] = UnderscoresToCamelCase(descriptor) + "Array";
                    (*variables)["number"]    = SimpleItoa(descriptor->number());
                    (*variables)["type"]      = ClassName(descriptor->message_type());
                    if(IsPrimitiveType(GetObjectiveCType(descriptor))) {
                        (*variables)["storage_type"]      = ClassName(descriptor->message_type());
                        (*variables)["storage_attribute"] = "";
                    } else {
                        (*variables)["storage_type"] = string(ClassName(descriptor->message_type())) + "*";
                        if(IsRetainedName(name)) {
                            (*variables)["storage_attribute"] = " NS_RETURNS_NOT_RETAINED";
                        } else {
                            (*variables)["storage_attribute"] = "";
                        }
                    }
                    (*variables)["group_or_message"] = (descriptor->type() == FieldDescriptor::TYPE_GROUP) ? "Group" : "Message";
                }
            } // namespace

            MessageFieldGenerator::MessageFieldGenerator(const FieldDescriptor *descriptor)
                : descriptor_(descriptor) {
                SetMessageVariables(descriptor, &variables_);
            }

            MessageFieldGenerator::~MessageFieldGenerator() {
            }

            void MessageFieldGenerator::GenerateHasFieldHeader(io::Printer *printer) const {
                printer->Print(variables_, "BOOL has$capitalized_name$_:1;\n");
            }

            void MessageFieldGenerator::GenerateFieldHeader(io::Printer *printer) const {
                printer->Print(variables_, "$storage_type$ $name$$storage_attribute$;\n");
            }

            void MessageFieldGenerator::GenerateHasPropertyHeader(io::Printer *printer) const {
                printer->Print(variables_, "- (BOOL)has$capitalized_name$;\n");
            }

            void MessageFieldGenerator::GeneratePropertyHeader(io::Printer *printer) const {
                printer->Print(variables_, "@property (nonatomic, readonly)$storage_attribute$ $storage_type$ $name$;\n");
            }

            void MessageFieldGenerator::GenerateExtensionSource(io::Printer *printer) const {
                printer->Print(variables_, "@property (nonatomic, readwrite) BOOL has$capitalized_name$;\n");
                printer->Print(variables_, "@property (nonatomic, readwrite)$storage_attribute$ $storage_type$ $name$;\n");
            }

            void MessageFieldGenerator::GenerateMembersHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateSynthesizeSource(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateInitializationSource(io::Printer *printer) const {
                printer->Print(variables_, "self.$name$ = [$type$ defaultInstance];\n");
            }

            void MessageFieldGenerator::GenerateBuilderMembersHeader(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($classname$_Builder*) set$capitalized_name$:($storage_type$) value;\n"
                    "- ($classname$_Builder*) set$capitalized_name$Builder:($type$_Builder*) builderForValue;\n"
                    "- ($classname$_Builder*) merge$capitalized_name$:($storage_type$) value;\n");
            }

            void MessageFieldGenerator::GenerateBuilderGetterHeader(io::Printer *printer) const {
                printer->Print(variables_, "- ($storage_type$) $name$;\n"
                                           "- (BOOL)has$capitalized_name$;\n");
            }

            void MessageFieldGenerator::GenerateBuilderClearHeader(io::Printer *printer) const {
                printer->Print(variables_, "- ($classname$_Builder*)clear$capitalized_name$;\n");
            }

            void MessageFieldGenerator::GenerateBuilderGetterSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($storage_type$) $name$ {\n"
                    "  return builder_result.$name$;\n"
                    "}\n");
                printer->Print(variables_,
                    "- (BOOL)has$capitalized_name$ {\n"
                    " return builder_result.has$capitalized_name$;\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateBuilderMembersSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($classname$_Builder*) set$capitalized_name$:($storage_type$) value {\n"
                    "  builder_result.has$capitalized_name$ = YES;\n"
                    "  builder_result.$name$ = value;\n"
                    "  return self;\n"
                    "}\n"
                    "- ($classname$_Builder*) set$capitalized_name$Builder:($type$_Builder*) builderForValue {\n"
                    "  return [self set$capitalized_name$:[builderForValue build]];\n"
                    "}\n"
                    "- ($classname$_Builder*) merge$capitalized_name$:($storage_type$) value {\n"
                    "  if (builder_result.has$capitalized_name$ &&\n"
                    "      builder_result.$name$ != [$type$ defaultInstance]) {\n"
                    "    builder_result.$name$ =\n"
                    "      [[[$type$ builderWithPrototype:builder_result.$name$] mergeFrom:value] buildPartial];\n"
                    "  } else {\n"
                    "    builder_result.$name$ = value;\n"
                    "  }\n"
                    "  builder_result.has$capitalized_name$ = YES;\n"
                    "  return self;\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateBuilderClearSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($classname$_Builder*)clear$capitalized_name$ {\n"
                    "  builder_result.has$capitalized_name$ = NO;\n"
                    "  builder_result.$name$ = [$type$ defaultInstance];\n"
                    "  return self;\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateMergingCodeHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateMergingCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "if (other.has$capitalized_name$) {\n"
                    "  [self merge$capitalized_name$:other.$name$];\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateBuildingCodeHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateBuildingCodeSource(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateParsingCodeHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateParsingCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "$type$_Builder* subBuilder = [$type$ builder];\n"
                    "if (self.builder_result.has$capitalized_name$) {\n"
                    "  [subBuilder mergeFrom:self.builder_result.$name$];\n"
                    "}\n");

                if(descriptor_->type() == FieldDescriptor::TYPE_GROUP) {
                    printer->Print(variables_,
                        "[input readGroup:$number$ builder:subBuilder extensionRegistry:extensionRegistry];\n");
                } else {
                    printer->Print(variables_,
                        "[input readMessage:subBuilder extensionRegistry:extensionRegistry];\n");
                }

                printer->Print(variables_,
                    "[self set$capitalized_name$:[subBuilder buildPartial]];\n");
            }

            void MessageFieldGenerator::GenerateSerializationCodeHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateSerializationCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "if (self.has$capitalized_name$) {\n"
                    "  [output write$group_or_message$:$number$ value:self.$name$];\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateSerializedSizeCodeHeader(io::Printer *printer) const {
            }

            void MessageFieldGenerator::GenerateSerializedSizeCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "if (self.has$capitalized_name$) {\n"
                    "  size_ += compute$group_or_message$Size($number$, self.$name$);\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateDescriptionCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "if (self.has$capitalized_name$) {\n"
                    "  [output appendFormat:@\"%@%@ {\\n\", indent, @\"$name$\"];\n"
                    "  [self.$name$ writeDescriptionTo:output\n"
                    "                       withIndent:[NSString stringWithFormat:@\"%@  \", indent]];\n"
                    "  [output appendFormat:@\"%@}\\n\", indent];\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateIsEqualCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "self.has$capitalized_name$ == otherMessage.has$capitalized_name$ &&\n"
                    "(!self.has$capitalized_name$ || [self.$name$ isEqual:otherMessage.$name$]) &&");
            }

            void MessageFieldGenerator::GenerateHashCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "if (self.has$capitalized_name$) {\n"
                    "  hashCode = hashCode * 31 + [self.$name$ hash];\n"
                    "}\n");
            }

            void MessageFieldGenerator::GenerateMembersSource(io::Printer *printer) const {
            }

            string MessageFieldGenerator::GetBoxedType() const {
                return ClassName(descriptor_->message_type());
            }

            RepeatedMessageFieldGenerator::RepeatedMessageFieldGenerator(const FieldDescriptor *descriptor)
                : descriptor_(descriptor) {
                SetMessageVariables(descriptor, &variables_);
            }

            RepeatedMessageFieldGenerator::~RepeatedMessageFieldGenerator() {
            }

            void RepeatedMessageFieldGenerator::GenerateHasFieldHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateFieldHeader(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "NSMutableArray * $list_name$;\n");
                } else {
                    printer->Print(variables_,
                        "PBAppendableArray * $list_name$;\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateHasPropertyHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GeneratePropertyHeader(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_, "@property (nonatomic, readonly, nullable) NSArray<$storage_type$> * $name$;\n");
                } else {
                    printer->Print(variables_, "@property (nonatomic, readonly, nullable) PBArray * $name$;\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateExtensionSource(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "@property (nonatomic, readwrite) NSMutableArray * $list_name$;\n");
                } else {
                    printer->Print(variables_,
                        "@property (strong) PBAppendableArray * $list_name$;\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateSynthesizeSource(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateInitializationSource(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateMembersHeader(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($storage_type$)$name$AtIndex:(NSUInteger)index;\n");
            }

            void RepeatedMessageFieldGenerator::GenerateMembersSource(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "- (NSArray *)$name$ {\n"
                        "  return self.$list_name$;\n"
                        "}\n"
                        "- ($storage_type$)$name$AtIndex:(NSUInteger)index {\n"
                        "  return self.$list_name$[index];\n"
                        "}\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderClearSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($classname$_Builder *)clear$capitalized_name$ {\n"
                    "  builder_result.$list_name$ = nil;\n"
                    "  return self;\n"
                    "}\n");
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderMembersHeader(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "- ($classname$_Builder *)add$capitalized_name$:($storage_type$)value;\n"
                        "- ($classname$_Builder *)set$capitalized_name$Array:(NSArray<$storage_type$> *)array NS_SWIFT_NAME(set$capitalized_name$Array(_:));\n"
                        "+ (Class)expectedElementTypeFor$capitalized_name$Array;\n");
                } else {
                    printer->Print(variables_,
                        "- ($classname$_Builder *)add$capitalized_name$:($storage_type$)value;\n"
                        "- ($classname$_Builder *)set$capitalized_name$Array:(NSArray<NSNumber *> *)array NS_SWIFT_NAME(set$capitalized_name$Array(_:));\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderGetterHeader(io::Printer *printer) const {
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_, "- (NSMutableArray *)$name$;\n");
                } else {
                    printer->Print(variables_, "- (PBAppendableArray *)$name$;\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderClearHeader(io::Printer *printer) const {
                printer->Print(variables_, "- ($classname$_Builder*)clear$capitalized_name$;\n");
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderGetterSource(io::Printer *printer) const {
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "- (NSMutableArray *)$name$ {\n"
                        "  return builder_result.$list_name$;\n"
                        "}\n");
                } else {
                    printer->Print(variables_,
                        "- (PBAppendableArray *)$name$ {\n"
                        "  return builder_result.$list_name$;\n"
                        "}\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateBuilderMembersSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "- ($classname$_Builder *)add$capitalized_name$:($storage_type$)value {\n"
                    "  if (builder_result.$list_name$ == nil) {\n"
                    "    builder_result.$list_name$ = [[NSMutableArray alloc]init];\n"
                    "  }\n"
                    "  [builder_result.$list_name$ addObject:value];\n"
                    "  return self;\n"
                    "}\n"
                    "- ($classname$_Builder *)set$capitalized_name$Array:(NSArray *)array {\n"
                    "  builder_result.$list_name$ = [[NSMutableArray alloc]initWithArray:array];\n"
                    "  return self;\n"
                    "}\n");
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "+ (Class)expectedElementTypeFor$capitalized_name$Array {\n"
                        "  return [$type$ class];\n"
                        "}\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateMergingCodeHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateBuildingCodeHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateParsingCodeHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateSerializationCodeHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateSerializedSizeCodeHeader(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateMergingCodeSource(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "if (other.$list_name$.count > 0) {\n"
                        "  builder_result.$list_name$ = [[NSMutableArray alloc] initWithArray:other.$list_name$];\n"
                        "}\n");
                } else {
                    printer->Print(variables_,
                        "if (other.$list_name$.count > 0) {\n"
                        "  builder_result.$list_name$ = [other.$list_name$ copy];\n"
                        "}\n");
                }
            }

            void RepeatedMessageFieldGenerator::GenerateBuildingCodeSource(io::Printer *printer) const {
            }

            void RepeatedMessageFieldGenerator::GenerateParsingCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "$type$_Builder* subBuilder = [$type$ builder];\n");

                if(descriptor_->type() == FieldDescriptor::TYPE_GROUP) {
                    printer->Print(variables_,
                        "[input readGroup:$number$ builder:subBuilder extensionRegistry:extensionRegistry];\n");
                } else {
                    printer->Print(variables_,
                        "[input readMessage:subBuilder extensionRegistry:extensionRegistry];\n");
                }

                printer->Print(variables_,
                    "[self add$capitalized_name$:[subBuilder buildPartial]];\n");
            }

            void RepeatedMessageFieldGenerator::GenerateSerializationCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "for ($type$ *element in self.$list_name$) {\n"
                    "  [output write$group_or_message$:$number$ value:element];\n"
                    "}\n");
            }

            void RepeatedMessageFieldGenerator::GenerateSerializedSizeCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "for ($type$ *element in self.$list_name$) {\n"
                    "  size_ += compute$group_or_message$Size($number$, element);\n"
                    "}\n");
            }

            void RepeatedMessageFieldGenerator::GenerateDescriptionCodeSource(io::Printer *printer) const {
                printer->Print(variables_,
                    "for ($type$* element in self.$list_name$) {\n"
                    "  [output appendFormat:@\"%@%@ {\\n\", indent, @\"$name$\"];\n"
                    "  [element writeDescriptionTo:output\n"
                    "                   withIndent:[NSString stringWithFormat:@\"%@  \", indent]];\n"
                    "  [output appendFormat:@\"%@}\\n\", indent];\n"
                    "}\n");
            }

            void RepeatedMessageFieldGenerator::GenerateIsEqualCodeSource(io::Printer *printer) const {
                printer->Print(variables_, "(self.$list_name$ == otherMessage.$list_name$ || [self.$list_name$ isEqualToArray:otherMessage.$list_name$]) &&");
            }

            void RepeatedMessageFieldGenerator::GenerateHashCodeSource(io::Printer *printer) const {
                //check if object array vs primitive array
                if(isObjectArray(descriptor_)) {
                    printer->Print(variables_,
                        "for ($type$* element in self.$list_name$) {\n"
                        "  hashCode = hashCode * 31 + [element hash];\n"
                        "}\n");
                } else {
                    GOOGLE_LOG(FATAL) << "Can't get here: GenerateHashCodeSource";
                }
            }

            string RepeatedMessageFieldGenerator::GetBoxedType() const {
                return ClassName(descriptor_->message_type());
            }
        } // namespace objectivec
    }     // namespace compiler
} // namespace protobuf
} // namespace google
