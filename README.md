# RCON

## Table of Contents

* [What is it ?](#what-is-it)
* [How to use it ?](#how-to-use-it)
* [How to build ?](#how-to-build)
	* [On Linux / macOS](#on-linux--macos)
	* [On Qindows](#on-windows)
* [TODO](#todo)

## What is it ?

This is a minecraft RCON client written in C for managing distant minecraft servers.

## How to use it ?

There is two option you can use:

	./rcon -H <host> -p <password>

By default, host is `127.0.0.1` and password is `minecraft`.

## How to build ?

This project is built using `cmake`.

### On Linux / macOS:

	cmake -B build .
	cd build
	make

### On Windows:

	cmake -B build .
	cmake --build build

## TODO

* Better CLI arguments parsing and handling
* Help information
* Remove garbage characters, mostly color codes
* Fix recv to read all received text