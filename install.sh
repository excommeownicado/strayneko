#!/usr/bin/env bash
set -euo pipefail

PREFIX=/usr
DESTDIR=
ICON_DIR=share/icons/hicolor/scalable/apps
DESKTOP_DIR=share/applications
BINARY_NAME=strayneko
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
  cat <<EOF
Usage: $0 [--prefix PREFIX] [--destdir DESTDIR]

Installs the strayneko binary, desktop launcher, and icon.

Options:
  --prefix PREFIX   Installation prefix (default: /usr)
  --destdir DIR     Temporary staging directory for packaging.
  -h, --help        Show this help message.
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --prefix)
      PREFIX="$2"
      shift 2
      ;;
    --destdir)
      DESTDIR="$2"
      shift 2
      ;;
    -h|--help)
      usage
      exit 0
      ;;
    *)
      echo "Unknown option: $1" >&2
      usage
      exit 1
      ;;
  esac
done

install_bin() {
  local src="$SCRIPT_DIR/build/$BINARY_NAME"
  local dst="$DESTDIR${PREFIX}/bin/$BINARY_NAME"

  if [[ ! -f "$src" ]]; then
    echo "Error: Built binary not found at $src" >&2
    echo "Please run cmake --build build before installing." >&2
    exit 1
  fi

  chmod +x "$src"
  mkdir -p "$(dirname "$dst")"
  install -Dm755 "$src" "$dst"
}

install_desktop() {
  local src="$SCRIPT_DIR/desktop/strayneko.desktop"
  local dst="$DESTDIR${PREFIX}/${DESKTOP_DIR}/strayneko.desktop"

  if [[ ! -f "$src" ]]; then
    echo "Error: Desktop entry not found at $src" >&2
    exit 1
  fi

  mkdir -p "$(dirname "$dst")"
  install -Dm644 "$src" "$dst"
}

install_icon() {
  local src="$SCRIPT_DIR/desktop/strayneko.svg"
  local dst="$DESTDIR${PREFIX}/${ICON_DIR}/strayneko.svg"

  if [[ ! -f "$src" ]]; then
    echo "Error: Icon file not found at $src" >&2
    exit 1
  fi

  mkdir -p "$(dirname "$dst")"
  install -Dm644 "$src" "$dst"
}

install_bin
install_desktop
install_icon

echo "strayneko installed to ${DESTDIR}${PREFIX}"
