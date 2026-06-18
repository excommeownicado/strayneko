# strayneko

Modernized version of the classic oneko desktop pet for X11 (also works on Wayland through XWayland).

Unlike the original neko, this version doesn't feature mouse-chasing behavior and instead makes neko wander around your desktop on their own.

## Features

* Random spawn point and autonomous wandering
* Multi-monitor support via XRandR
* Optional monitor restriction
* Adjustable idle time between walks
* Zoomies mode
* Cat and dog sprites

## Building

```bash
xmkmf -a
make
sudo make install
```
## Usage

* `--monitor` – restricts neko to a specified monitor
* `--min-wait` – sets the minimum idle time between walks
* `--max-wait` – sets the maximum idle time between walks

<details>
<summary>Usage examples</summary>
  
```bash
oneko --monitor 0
oneko --min-wait 10 --max-wait 100
```

</details>

## Credits

This project is based on oneko 1.2.sakura.5

Original project:
http://www.daidouji.com/oneko/

> [!NOTE]
> This repository is an unofficial fork and is not affiliated with the original authors.


