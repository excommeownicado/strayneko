# strayneko

Modernized version of the classic neko desktop pet for X11 (also works on Wayland through XWayland).

Unlike the original neko, this version doesn't feature mouse-chasing behavior and instead makes neko wander around your desktop on their own.

## Features

* Random spawn point and autonomous wandering
* Multi-monitor support via XRandR
* Optional monitor restriction
* Adjustable idle time between walks
* Zoomies mode
* Optional draggable bed (`--bed`)

## Building with CMake

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

To install:

```bash
sudo cmake --install .
```

## Usage

* `--monitor` – restricts neko to a specified monitor
* `--min-wait` – sets the minimum idle time between walks
* `--max-wait` – sets the maximum idle time between walks
* `--bed` – shows a draggable bed that neko may occasionally visit

<details>
<summary>Usage examples</summary>
  
```bash
strayneko --monitor 0
strayneko --min-wait 10 --max-wait 100
```

</details>

## Credits

This project is based on oneko 1.2.sakura.5

Original project:
http://www.daidouji.com/oneko/

Fork maintainer: excommeownicado

## License

This fork is released under the MIT License. See the LICENSE file for details.

The original upstream code includes portions that are Public Domain; where
applicable, original authors' public-domain notices are preserved in the
source and credits.

> [!NOTE]
> This repository is an unofficial fork and is not affiliated with the original authors.

