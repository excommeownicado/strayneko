# Changelog

All notable changes to this project are documented here.

## Unreleased

### Fixed

* Fixed possible integer overflow in configuration option values.
* Fixed an issue where X11 error handler did not correctly request shutdown.
* Fixed neko clamping on multi-monitor setups. 

### Improved

* Improved error handling.
* Improved code robustness.
* Improved monitor geometry arithmetic.

## [1.0.2]

### Fixed

* Fixed an issue where sprite animation did not update correctly.
* Fixed invalid floating-point configuration values such as `inf` and `nan`.
* Fixed monitor resource cleanup.
* Fixed bed position file handling.
* Fixed movement direction calculation at angle boundaries.

### Improved

* Improved monitor geometry handling.
* Improved configuration validation.
* Improved target and movement handling.

## [1.0.1]

### Added

* Added an uninstaller.

### Changed

* Removed `README.md` and `CREDITS.md` from release archives.

### Improved

* Improved and updated the man page.

## [1.0.0]

Initial release.
