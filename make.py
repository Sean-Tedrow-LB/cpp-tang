#!/usr/bin/env python3


source_files = [
    "main.cpp",
    "tang_argument_parser.cpp",
    "tang_comment_removal.cpp",
    "tang_file_reader.cpp",
    "tang_file_writer.cpp",
    "ix_unicode.cpp",
    "tang_modules.cpp"
]







linux_flags = [
    "-std=c++11",
    "-Wall",
    "-fno-exceptions"
]

linux_libs = [
]






windows_flags = [
    "/D", "_UNICODE",
    "/D", "UNICODE",
    "/EHsc"
]

windows_libs = [
]










import subprocess
import sys
import os
import shutil



def source_files_to_paths(source_files):
    paths = []
    for f in source_files:
        paths.append(os.path.join("src", f))
    return paths

def compile_for_windows(files, debug, additional_flags, out_path):
    paths = source_files_to_paths(files)
    defines = []
    if debug:
        additional_flags += ["/MTd"]
    else:
        additional_flags += ["/MT", "/O2"]
        defines += ["/DNDEBUG"]
    command = ["cl"] + defines + ["/I", "windows_include", "/Fo" + "obj\\"] + \
               windows_flags + additional_flags + paths + windows_libs + \
               ["/Fe" + out_path, "/link", "/SUBSYSTEM:CONSOLE"]
    print("\n\n\n", command, "\n\n\n", sep="", end="")
    subprocess.call(command)

def compile_for_linux(files, debug, additional_flags, out_path):
    paths = source_files_to_paths(files)
    defines = []
    if debug:
        additional_flags += ["-g"]
    else:
        defines += ["-D", "NDEBUG"]
        additional_flags += ["-O3", "-s"]
    command = ["g++"] + linux_flags + additional_flags + defines + \
                    paths + linux_libs + ["-o", out_path]
    print("\n\n\n", command, "\n\n\n", sep="", end="")
    subprocess.call(command)


debug = False
if len(sys.argv) > 1:
    debug = (sys.argv[1] == "debug")
additional_flags = []
if debug:
    additional_flags = sys.argv[2:]
else:
    additional_flags = sys.argv[1:]
out_path = os.path.join("bin", "tangc")


if sys.platform.startswith("linux"):
    compile_for_linux(source_files, debug, additional_flags, out_path)
elif sys.platform.startswith("win32") or sys.platform.startswith("cygwin"):
    compile_for_windows(source_files, debug, additional_flags, out_path)
else:
    print(sys.argv[0], "is for Windows and Linux only, use the XCode project to build for Mac OS X")
