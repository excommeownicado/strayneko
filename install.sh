#!/usr/bin/env bash
set -euo pipefail

PREFIX=/usr
PREFIX_SET=false
DESTDIR=
ICON_DIR=share/icons/hicolor/scalable/apps
DESKTOP_DIR=share/applications
MAN_DIR=share/man/man6
BINARY_NAME=strayneko
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

usage() {
  cat <<EOF
Usage: $0 [--prefix PREFIX] [--destdir DESTDIR]

Installs the release-ready strayneko binary, desktop launcher, icon, and man page.

Options:
  --prefix PREFIX   Installation prefix (default: /usr; without root falls back to ~/.local)
  --destdir DIR     Temporary staging directory for packaging.
  -h, --help        Show this help message.
EOF
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --prefix)
      PREFIX="$2"
      PREFIX_SET=true
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

if [[ $EUID -ne 0 && $PREFIX == "/usr" && $PREFIX_SET == false ]]; then
  PREFIX="$HOME/.local"
  echo "No root privileges detected; installing to $PREFIX instead." >&2
elif [[ $EUID -ne 0 && $PREFIX == "/usr" ]]; then
  echo "Warning: not running as root; installing to $PREFIX may fail unless you have write access." >&2
fi

install_bin() {
  local candidates=(
    "$SCRIPT_DIR/$BINARY_NAME"
    "$SCRIPT_DIR/build/$BINARY_NAME"
    "$SCRIPT_DIR/src/$BINARY_NAME"
    "$SCRIPT_DIR/release/$BINARY_NAME"
  )

  local src=""
  for candidate in "${candidates[@]}"; do
    if [[ -f "$candidate" ]]; then
      src="$candidate"
      break
    fi
  done

  if [[ -z "$src" ]]; then
    echo "Error: Binary not found in release package or build directory." >&2
    echo "Searched paths:" >&2
    for candidate in "${candidates[@]}"; do
      echo "  - $candidate" >&2
    done
    echo "If you are using the source repository, build it first with cmake --build build." >&2
    exit 1
  fi

  chmod +x "$src"
  local dst="$DESTDIR${PREFIX}/bin/$BINARY_NAME"
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

install_man() {
  local src="$SCRIPT_DIR/strayneko.man"
  if [[ ! -f "$src" ]]; then
    return
  fi

  local dst="$DESTDIR${PREFIX}/${MAN_DIR}/strayneko.man"
  mkdir -p "$(dirname "$dst")"
  install -Dm644 "$src" "$dst"
}

install_bin
install_desktop
install_icon
install_man
