<p align="center">
  <img src="logo.svg" alt="strayneko" width="500">
</p>

Modern version of the classic neko desktop pet for X11. It also works on Wayland through XWayland.

Unlike the original oneko, strayneko doesn't constantly chase your mouse cursor. Instead, it wanders around your desktop on its own.

## Features

* Autonomous wandering
* Random spawn position
* Multi-monitor support
* Optional monitor restriction
* Configurable idle time between walks
* Zoomies mode
* Optional draggable bed with saved position

## Installation

### From a release

Download the latest release, extract the archive, and run:

```bash
./install.sh
```

The installer will automatically install the binary, desktop entry, and application icon.

### Build from source

```bash
git clone https://github.com/excommeownicado/strayneko.git
cd strayneko
```

```bash
cmake -B build
cmake --build build
```

To install system-wide:

```bash
sudo cmake --install build
```

## Usage

Available command-line options:

* `--monitor` – restricts neko to a specified monitor
* `--min-wait` – sets the minimum idle time between walks
* `--max-wait` – sets the maximum idle time between walks
* `--bed` – shows a draggable bed that neko may occasionally visit

<details>
<summary>Usage examples</summary>
  
```bash
strayneko --monitor 0 --bed
strayneko --min-wait 10 --max-wait 100
```

</details>

## Credits

This project is based on oneko 1.2.sakura.5.

Original project:
http://www.daidouji.com/oneko/

Modern fork and additional features:
excommeownicado

## License

This fork is released under the MIT License. See the LICENSE file for details.

The original upstream code includes portions that are Public Domain; where
applicable, original authors' public-domain notices are preserved in the
source and credits.

> [!NOTE]
> This project is not affiliated with the original authors.

