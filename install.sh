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

find_existing() {
  for path in "$@"; do
    [[ -f "$path" ]] && {
      printf '%s\n' "$path"
      return 0
    }
  done
  return 1
}

while [[ $# -gt 0 ]]; do
  case "$1" in
    --prefix)
      if [[ $# -lt 2 ]]; then
          echo "Error: --prefix requires a value." >&2
          exit 1
      fi
      PREFIX="$2"
      PREFIX_SET=true
      shift 2
      ;;
    --destdir)
      if [[ $# -lt 2 ]]; then
          echo "Error: --destdir requires a value." >&2
          exit 1
      fi
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
  local src
  src=$(find_existing \
    "$SCRIPT_DIR/$BINARY_NAME" \
    "$SCRIPT_DIR/build/src/$BINARY_NAME" \
    "$SCRIPT_DIR/build/$BINARY_NAME" \
    "$SCRIPT_DIR/build/bin/$BINARY_NAME") || {
      echo "Error: Binary not found." >&2
      echo "Looked in:" >&2
      echo "  $SCRIPT_DIR/$BINARY_NAME" >&2
      echo "  $SCRIPT_DIR/build/src/$BINARY_NAME" >&2
      echo "  $SCRIPT_DIR/build/$BINARY_NAME" >&2
      echo "If you are using the source repository, build it first with:"
      echo "  cmake -B build"
      echo "  cmake --build build"
      exit 1
    }

  chmod +x "$src"

  local dst="$DESTDIR${PREFIX}/bin/$BINARY_NAME"

  install -Dm755 "$src" "$dst"
}

install_desktop() {
  local src
  src=$(find_existing \
    "$SCRIPT_DIR/resources/desktop/strayneko.desktop" \
    "$SCRIPT_DIR/desktop/strayneko.desktop") || {
      echo "Error: Desktop entry not found." >&2
      exit 1
    }

  local dst="$DESTDIR${PREFIX}/${DESKTOP_DIR}/strayneko.desktop"

  install -Dm644 "$src" "$dst"

  sed -i "s|^Exec=.*|Exec=$PREFIX/bin/$BINARY_NAME|" "$dst"
}

install_icon() {
  local src
  src=$(find_existing \
    "$SCRIPT_DIR/resources/desktop/strayneko.svg" \
    "$SCRIPT_DIR/desktop/strayneko.svg") || {
      echo "Error: Icon not found." >&2
      exit 1
    }

  local dst="$DESTDIR${PREFIX}/${ICON_DIR}/strayneko.svg"

  install -Dm644 "$src" "$dst"
}

install_man() {
  local src
  src=$(find_existing \
    "$SCRIPT_DIR/docs/strayneko.6" \
    "$SCRIPT_DIR/strayneko.6") || {
    echo "Error: Man page not found." >&2
    exit 1
  }

  local dst="$DESTDIR${PREFIX}/${MAN_DIR}/strayneko.6"

  install -Dm644 "$src" "$dst"
}

install_uninstaller() {
  local src
  src=$(find_existing \
    "$SCRIPT_DIR/uninstall.sh" \
    "$SCRIPT_DIR/scripts/uninstall.sh") || {
    echo "Error: Uninstaller not found." >&2
    exit 1
  }

  local dst="$DESTDIR${PREFIX}/bin/strayneko-uninstall"

  install -Dm755 "$src" "$dst"
}

install_bin
install_desktop
install_icon
install_man
install_uninstaller

echo
echo "Strayneko has been installed successfully."
echo
echo "Run it with:"
echo "  strayneko"
echo
echo "For available options:"
echo "  strayneko --help"
echo
echo "To remove it later, run:"
echo "  strayneko-uninstall"
echo