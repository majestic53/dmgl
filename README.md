# DMGL (DMG-Lite)

[![License: MIT](https://shields.io/badge/license-MIT-blue.svg?style=flat)](https://github.com/majestic53/dmgl/blob/master/LICENSE.md) [![Build Status](https://github.com/majestic53/dmgl/workflows/Build/badge.svg)](https://github.com/majestic53/dmgl/actions/workflows/build.yml)

### A lightweight Gameboy emulator, written in C.

The goal of `DMGL` is to emulate the original Gameboy hardware with enough accuracy to run most common games, while being performant and natively compilable across multiple platforms.

__NOTE__: `DMGL` is a still a work-in-progress. Some features may be incomplete or may contain bugs.

## Building

`DMGL` is dependant on the `SDL2` library. If you are on a Debian-based systems, install this library using apt:

```bash
sudo apt-get install libsdl2-dev
```
If you are on an Arch-based systems, install this library using pacman:

```bash
sudo pacman -S sdl2
```

Once all dependencies are met, clone the repo and build:

```bash
git clone https://github.com/majestic53/dmgl
cd dmgl
sudo make
```

## Usage

```
Usage: dmgl [options] file...

Options:
   -b, --bootloader   Specify bootloader path
   -h, --help         Show help information
   -v, --version      Show version information
```

### Examples

```bash
# To launch with a cartridge, run the following command
dmgl cart.gb

# To launch with a bootloader, run the following command
dmgl -b boot.gb cart.gb
```

## Keybindings

The following keybindings are available:

|Key  |Button       |
|:----|:------------|
|L    |A button     |
|K    |B button     |
|C    |Select button|
|Space|Start button |

|Key  |D-pad          |
|:----|:--------------|
|W    |Up direction   |
|A    |Left direction |
|S    |Down direction |
|D    |Right direction|

## Mapper support

The following mappers are supported:

|# |Mapper                                     |Description|
|:-|:------------------------------------------|:----------|
|0 |[MBC0](https://gbdev.io/pandocs/nombc.html)|No mapper  |

## License

Copyright (C) 2022 David Jolly. Released under the [MIT License](https://github.com/majestic53/dmgl/blob/master/LICENSE.md).