#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# This file is part of the python-chess library.
# Copyright (C) 2012 Niklas Fiekas <niklas.fiekas@tu-clausthal.de>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import distutils
import distutils.sysconfig
import os
from distutils.core import setup, Extension

def read(filename):
    """Utility function that returns a files contents."""
    return open(os.path.join(os.path.dirname(__file__), filename)).read()

# Work around: -Wstrict-prototypes flag is invalid for C++.
(opt, ) = distutils.sysconfig.get_config_vars("OPT")
if opt:
    os.environ["OPT"] = " ".join(flag for flag in opt.split()
                                 if flag != "-Wstrict-prototypes")

# Module description.
setup(
    name="python-chess",
    version="0.0.5",
    author="Niklas Fiekas",
    author_email="niklas.fiekas@tu-clausthal.de",
    description="A chess library.",
    long_description=read("README.rst"),
    license="GPL3",
    keywords="chess fen pgn polyglot",
    url="http://github.com/niklasf/python-chess",
    packages=["chess"],
    scripts=["scripts/ecotool.py"],
    ext_modules=[
        Extension(
            name="libchess",
            sources=[
                "libchess/libchess.cc",
                "libchess/piece.cc",
                "libchess/square.cc",
                "libchess/move.cc",
                "libchess/move_info.cc",
                "libchess/position.cc",
                "libchess/attacker_generator.cc",
                "libchess/legal_move_generator.cc",
                "libchess/pseudo_legal_move_generator.cc",
                "libchess/polyglot_opening_book_entry.cc",
                "libchess/game_header_bag.cc",
                "libchess/pgn_index.cc",
            ],
            libraries=[
                "boost_python27",
                "boost_regex",
                "boost_filesystem",
            ],
        ),
    ],
    classifiers=[
        "Development Status :: 2 - Pre-Alpha",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: GNU General Public License (GPL)",
        "Operating System :: OS Independent",
        "Programming Language :: Python",
        "Programming Language :: C++",
        "Topic :: Games/Entertainment :: Board Games",
        "Topic :: Software Development :: Libraries :: Python Modules",
    ],
)
