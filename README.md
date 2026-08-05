<p align="center">
  <img src="assets/logo.svg" alt="strayneko" width="500">
</p>

<p align="center">

<img alt="linux" src="https://img.shields.io/badge/Linux-262c33?logo=linux&style=for-the-badge&logoColor=white">
<img alt="xwayland" src="https://img.shields.io/badge/X11_·_XWayland-262c33?style=for-the-badge">
<img alt="cmake" src="https://img.shields.io/badge/CMake-3.16+-064F8C?labelColor=262c33&logo=cmake&style=for-the-badge">

</p>

Modern take on the classic neko desktop pet for X11. Works on Wayland through XWayland.

Unlike the original oneko, strayneko doesn't constantly chase your mouse cursor. Instead, it wanders around your desktop on its own.

Originally created for personal use, but shared in case someone else finds it useful or enjoyable.

## Features

<img align="right" src="assets/demo.gif" height="100%">

* Autonomous wandering
* Random spawn position
* Zoomies mode
* Optional draggable bed with saved position
* Configurable idle time between walks
* Multi-monitor support
* Optional monitor restriction

## Installation

### From a release (recommended)

Download the [latest release](https://github.com/excommeownicado/strayneko/releases/latest), extract the archive, and run:

```bash
./install.sh
```

The installer will automatically install the binary, desktop entry, and application icon.

---

### Build from source

```bash
git clone https://github.com/excommeownicado/strayneko.git
cd strayneko
```

```bash
cmake -B build
cmake --build build
```

<details>
  
<summary>For developers</summary>
  
To enable developer-only options and debug logging, build with:

```bash
cmake -B build \
      -DCMAKE_BUILD_TYPE=Debug \
      -DENABLE_DEBUG=ON

cmake --build build
```

</details>

To install system-wide:

```bash
sudo cmake --install build
```

## Usage

Simply execute `strayneko` to go with a default look and behavior. For customization use the following command-line arguments.

Available command-line options:

* `--fg` – changes color of neko's outline
* `--bg` – changes color of neko's body
* `--bed` – shows a draggable bed that neko may occasionally visit
* `--min-wait` – sets the minimum idle time between walks
* `--max-wait` – sets the maximum idle time between walks
* `--monitor` – restricts neko to a specified monitor

<details>
  
<summary>Developer options</summary>

If debug mode is enabled (only available through building from source):
  
* `--debug` – enables debug log output
* `--force-target` – forces neko to go to the specified coordinates

</details>

<details open>

<summary>Examples</summary>
  
```bash
strayneko --monitor 0 --bed
strayneko --min-wait 10 --max-wait 100
```

</details>

## Credits

This project is based on **oneko 1.2.sakura.5**.

Original project:    
http://www.daidouji.com/oneko/

See [CREDITS](CREDITS.md) for full attribution.

## License

This fork is released under the MIT License. See the [LICENSE](LICENSE) file for details.

The original upstream code includes portions that are Public Domain; where
applicable, original authors' public-domain notices are preserved in the
source and credits.

> [!NOTE]
> This project is an idependent fork and is not affiliated with the original authors.

