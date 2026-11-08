# RCON

[![Build](https://github.com/Unimat45/Rcon/actions/workflows/build.yml/badge.svg)](https://github.com/Unimat45/Rcon/actions/workflows/build.yml)
[![License: GPL v3+](https://img.shields.io/badge/License-GPLv3%2B-blue.svg)](LICENSE)

## Table of Contents

* [What is it ?](#what-is-it)
* [How to use it ?](#how-to-use-it)
* [How to build ?](#how-to-build)
	* [On Linux / macOS](#on-linux--macos)
	* [On Windows](#on-windows)
* [License](#license)

## What is it ?

This is a minecraft RCON client written in C for managing distant minecraft servers.

## How to use it ?

Two clients are available: a Qt interface and a cli application

There are two arguments you can use with the cli:

	./rcon -H <host> -p <password>

By default, host is `127.0.0.1` and password is `minecraft`.

## How to build ?

This project is built using `cmake`.
There are two options available to customize the build

    -DBUILD_QT=ON
    -DBUILD_CLI=ON

### On Linux / macOS:

	cmake -B build .
	make -j$(nproc) -C build

### On Windows:

	cmake -B build .
	cmake --build build

## License

Copyright (C) 2023-2026 Mathieu Déry

This program is free software: you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the [LICENSE](LICENSE) file for the full text.

Releases up to and including v2.0.0 were published under the MIT License.

Bundled third-party code (argparse, nlohmann/json) keeps its own MIT license;
see [THIRD_PARTY_NOTICES.md](THIRD_PARTY_NOTICES.md).
