#!/usr/bin/env python3

import logging
import argparse
import subprocess
import io
import os
import sys
import re
import struct
import datetime

ICONS_SUPPORTED_FORMATS = ["png"]

ICONS_TEMPLATE_H_HEADER = """#pragma once

#include <gui/icon.h>

typedef enum {
"""
ICONS_TEMPLATE_H_ICON_NAME = "\t{name},\n"
ICONS_TEMPLATE_H_FOOTER = """} IconName;

Icon * assets_icons_get(IconName name);
"""

ICONS_TEMPLATE_H_I = """#pragma once

#include <assets_icons.h>

const IconData * assets_icons_get_data(IconName name);
"""

ICONS_TEMPLATE_C_HEADER = """#include \"assets_icons_i.h\"
#include <gui/icon_i.h>

"""
ICONS_TEMPLATE_C_FRAME = "const uint8_t {name}[] = {data};\n"
ICONS_TEMPLATE_C_DATA = "const uint8_t *{name}[] = {data};\n"
ICONS_TEMPLATE_C_ICONS_ARRAY_START = "const IconData icons[] = {\n"
ICONS_TEMPLATE_C_ICONS_ITEM = "\t{{ .width={width}, .height={height}, .frame_count={frame_count}, .frame_rate={frame_rate}, .frames=_{name} }},\n"
ICONS_TEMPLATE_C_ICONS_ARRAY_END = "};"
ICONS_TEMPLATE_C_FOOTER = """
const IconData * assets_icons_get_data(IconName name) {
    return &icons[name];
}

Icon * assets_icons_get(IconName name) {
    return icon_alloc(assets_icons_get_data(name));
}
"""


class Assets:
    def __init__(self):
        # command args
        self.parser = argparse.ArgumentParser()
        self.parser.add_argument("-d", "--debug", action="store_true", help="Debug")
        self.subparsers = self.parser.add_subparsers(help="sub-command help")
        self.parser_icons = self.subparsers.add_parser(
            "icons", help="Process icons and build icon registry"
        )
        self.parser_icons.add_argument(
            "-s", "--source-directory", help="Source directory"
        )
        self.parser_icons.add_argument(
            "-o", "--output-directory", help="Output directory"
        )
        self.parser_icons.set_defaults(func=self.icons)
        self.parser_otp = self.subparsers.add_parser(
            "otp", help="OTP HW version generator"
        )
        self.parser_otp.add_argument(
            "--version", type=int, help="Version", required=True
        )
        self.parser_otp.add_argument(
            "--firmware", type=int, help="Firmware", required=True
        )
        self.parser_otp.add_argument("--body", type=int, help="Body", required=True)
        self.parser_otp.add_argument(
            "--connect", type=int, help="Connect", required=True
        )
        self.parser_otp.add_argument("--name", type=str, help="Name", required=True)
        self.parser_otp.add_argument("file", help="Output file")
        self.parser_otp.set_defaults(func=self.otp)
        # logging
        self.logger = logging.getLogger()

    def __call__(self):
        self.args = self.parser.parse_args()
        if "func" not in self.args:
            self.parser.error("Choose something to do")
        # configure log output
        self.log_level = logging.DEBUG if self.args.debug else logging.INFO
        self.logger.setLevel(self.log_level)
        self.handler = logging.StreamHandler(sys.stdout)
        self.handler.setLevel(self.log_level)
        self.formatter = logging.Formatter("%(asctime)s [%(levelname)s] %(message)s")
        self.handler.setFormatter(self.formatter)
        self.logger.addHandler(self.handler)
        # execute requested function
        self.args.func()

    def otp(self):
        self.logger.debug(f"Generating OTP")

        if self.args.name:
            name = re.sub(
                "[^a-zA-Z0-9.]", "", self.args.name
            )  # Filter all special characters
            name = list(
                map(str, map(ord, name[0:8]))
            )  # Strip to 8 chars and map to ascii codes
            while len(name) < 8:
                name.append("0")

            n1, n2, n3, n4, n5, n6, n7, n8 = map(int, name)

        data = struct.pack(
            "<BBBBLBBBBBBBB",
            self.args.version,
            self.args.firmware,
            self.args.body,
            self.args.connect,
            int(datetime.datetime.now().timestamp()),
            n1,
            n2,
            n3,
            n4,
            n5,
            n6,
            n7,
            n8,
        )
        open(self.args.file, "wb").write(data)

    def icons(self):
        self.logger.debug(f"Converting icons")
        icons_c = open(os.path.join(self.args.output_directory, "assets_icons.c"), "w")
        icons_c.write(ICONS_TEMPLATE_C_HEADER)
        icons = []
        # Traverse icons tree, append image data to source file
        for dirpath, dirnames, filenames in os.walk(self.args.source_directory):
            self.logger.debug(f"Processing directory {dirpath}")
            if not filenames:
                continue
            if "frame_rate" in filenames:
                self.logger.debug(f"Folder contatins animation")
                icon_name = "A_" + os.path.split(dirpath)[1].replace("-", "_")
                width = height = None
                frame_count = 0
                frame_rate = 0
                frame_names = []
                for filename in sorted(filenames):
                    fullfilename = os.path.join(dirpath, filename)
                    if filename == "frame_rate":
                        frame_rate = int(open(fullfilename, "r").read().strip())
                        continue
                    elif not self.iconIsSupported(filename):
                        continue
                    self.logger.debug(f"Processing animation frame {filename}")
                    temp_width, temp_height, data = self.icon2header(fullfilename)
                    if width is None:
                        width = temp_width
                    if height is None:
                        height = temp_height
                    assert width == temp_width
                    assert height == temp_height
                    frame_name = f"_{icon_name}_{frame_count}"
                    frame_names.append(frame_name)
                    icons_c.write(
                        ICONS_TEMPLATE_C_FRAME.format(name=frame_name, data=data)
                    )
                    frame_count += 1
                assert frame_rate > 0
                assert frame_count > 0
                icons_c.write(
                    ICONS_TEMPLATE_C_DATA.format(
                        name=f"_{icon_name}", data=f'{{{",".join(frame_names)}}}'
                    )
                )
                icons_c.write("\n")
                icons.append((icon_name, width, height, frame_rate, frame_count))
            else:
                # process icons
                for filename in filenames:
                    if not self.iconIsSupported(filename):
                        continue
                    self.logger.debug(f"Processing icon {filename}")
                    icon_name = "I_" + "_".join(filename.split(".")[:-1]).replace(
                        "-", "_"
                    )
                    fullfilename = os.path.join(dirpath, filename)
                    width, height, data = self.icon2header(fullfilename)
                    frame_name = f"_{icon_name}_0"
                    icons_c.write(
                        ICONS_TEMPLATE_C_FRAME.format(name=frame_name, data=data)
                    )
                    icons_c.write(
                        ICONS_TEMPLATE_C_DATA.format(
                            name=f"_{icon_name}", data=f"{{{frame_name}}}"
                        )
                    )
                    icons_c.write("\n")
                    icons.append((icon_name, width, height, 0, 1))
        # Create array of images:
        self.logger.debug(f"Finalizing source file")
        icons_c.write(ICONS_TEMPLATE_C_ICONS_ARRAY_START)
        for name, width, height, frame_rate, frame_count in icons:
            icons_c.write(
                ICONS_TEMPLATE_C_ICONS_ITEM.format(
                    name=name,
                    width=width,
                    height=height,
                    frame_rate=frame_rate,
                    frame_count=frame_count,
                )
            )
        icons_c.write(ICONS_TEMPLATE_C_ICONS_ARRAY_END)
        icons_c.write(ICONS_TEMPLATE_C_FOOTER)
        icons_c.write("\n")
        # Create Public Header
        self.logger.debug(f"Creating header")
        icons_h = open(os.path.join(self.args.output_directory, "assets_icons.h"), "w")
        icons_h.write(ICONS_TEMPLATE_H_HEADER)
        for name, width, height, frame_rate, frame_count in icons:
            icons_h.write(ICONS_TEMPLATE_H_ICON_NAME.format(name=name))
        icons_h.write(ICONS_TEMPLATE_H_FOOTER)
        # Create Private Header
        icons_h_i = open(
            os.path.join(self.args.output_directory, "assets_icons_i.h"), "w"
        )
        icons_h_i.write(ICONS_TEMPLATE_H_I)
        self.logger.debug(f"Done")

    def icon2header(self, file):
        output = subprocess.check_output(["convert", file, "xbm:-"])
        assert output
        f = io.StringIO(output.decode().strip())
        width = int(f.readline().strip().split(" ")[2])
        height = int(f.readline().strip().split(" ")[2])
        data = f.read().strip().replace("\n", "").replace(" ", "").split("=")[1][:-1]
        return width, height, data

    def iconIsSupported(self, filename):
        extension = filename.lower().split(".")[-1]
        return extension in ICONS_SUPPORTED_FORMATS


if __name__ == "__main__":
    Assets()()
