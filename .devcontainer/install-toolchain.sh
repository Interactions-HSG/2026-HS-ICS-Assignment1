#!/usr/bin/env bash
# Installs the ARM GNU toolchain 13.3.Rel1 (arm-none-eabi) into the Codespace.
#
# NOTE: this is here so you can compile and sanity-check your C code in the browser.
# You still need the toolchain on your OWN machine for Task 2's screenshot and to
# write kernel7l.img onto a physical SD card -- a Codespace cannot reach your SD card.
set -euo pipefail

VERSION="13.3.rel1"
PREFIX="/opt/arm-gnu-toolchain"
BASE="https://developer.arm.com/-/media/Files/downloads/gnu/${VERSION}/binrel"

case "$(uname -m)" in
  x86_64)          ARCH="x86_64"  ;;
  aarch64|arm64)   ARCH="aarch64" ;;
  *) echo "Unsupported architecture: $(uname -m)" >&2; exit 1 ;;
esac

TARBALL="arm-gnu-toolchain-${VERSION}-${ARCH}-arm-none-eabi.tar.xz"

if [ -x "${PREFIX}/bin/arm-none-eabi-gcc" ]; then
  echo "Toolchain already installed at ${PREFIX}"
  exit 0
fi

echo "Installing build dependencies..."
sudo apt-get update -qq
sudo apt-get install -y -qq --no-install-recommends make xz-utils curl python3 libncurses-dev

echo "Downloading ${TARBALL} (~150 MB, this takes a minute)..."
curl -fL --retry 3 -o "/tmp/${TARBALL}" "${BASE}/${TARBALL}"

echo "Unpacking to ${PREFIX}..."
sudo mkdir -p "${PREFIX}"
sudo tar -xJf "/tmp/${TARBALL}" -C "${PREFIX}" --strip-components=1
rm -f "/tmp/${TARBALL}"

# Put the toolchain on PATH for every shell in the container.
echo "export PATH=\"${PREFIX}/bin:\$PATH\"" | sudo tee /etc/profile.d/arm-toolchain.sh >/dev/null
sudo chmod +x /etc/profile.d/arm-toolchain.sh
for f in "${PREFIX}"/bin/arm-none-eabi-*; do
  sudo ln -sf "$f" "/usr/local/bin/$(basename "$f")"
done

echo
"${PREFIX}/bin/arm-none-eabi-gcc" --version | head -1
echo "Toolchain ready. Try: make led_blink"
