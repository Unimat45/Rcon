# RCON

## Table of Contents

* [What is it ?](#what-is-it)
* [How to use it ?](#how-to-use-it)
* [How to build ?](#how-to-build)
	* [On Linux / macOS](#on-linux--macos)
	* [On Windows](#on-windows)

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
