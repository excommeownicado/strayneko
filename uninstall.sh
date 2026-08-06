#!/usr/bin/env bash
set -euo pipefail

PREFIX=/usr
PREFIX_SET=false
DESTDIR=

ICON_DIR=share/icons/hicolor/scalable/apps
DESKTOP_DIR=share/applications
MAN_DIR=share/man/man6
BINARY_NAME=strayneko

usage() {
  cat <<EOF
Usage: $0 [--prefix PREFIX] [--destdir DESTDIR]

Removes strayneko and all files installed by install.sh.

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
elif [[ $EUID -ne 0 && $PREFIX == "/usr" ]]; then
  echo "Warning: not running as root; removing from $PREFIX may fail unless you have write access." >&2
fi

remove_file() {
  local file="$1"

  if [[ -e "$file" ]]; then
    rm -f "$file"
    echo "Removed: $file"
  fi
}

remove_file "$DESTDIR${PREFIX}/bin/$BINARY_NAME"
remove_file "$DESTDIR${PREFIX}/${DESKTOP_DIR}/strayneko.desktop"
remove_file "$DESTDIR${PREFIX}/${ICON_DIR}/strayneko.svg"
remove_file "$DESTDIR${PREFIX}/${MAN_DIR}/strayneko.6"

if command -v update-desktop-database >/dev/null 2>&1; then
  update-desktop-database "$DESTDIR${PREFIX}/${DESKTOP_DIR}" >/dev/null 2>&1 || true
fi

if command -v gtk-update-icon-cache >/dev/null 2>&1; then
  gtk-update-icon-cache -q "$DESTDIR${PREFIX}/share/icons/hicolor" >/dev/null 2>&1 || true
fi

if command -v mandb >/dev/null 2>&1; then
  mandb >/dev/null 2>&1 || true
fi

echo "Uninstall complete."