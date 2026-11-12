name: Build PS4 app

on:
  workflow_dispatch:
  push:

jobs:
  build:
    runs-on: ubuntu-22.04
    steps:
      - uses: actions/checkout@v4

      - name: Install tools
        run: |
          sudo apt-get update
          sudo apt-get install -y clang lld

      - name: Download OpenOrbis toolchain
        run: |
          curl -sL https://api.github.com/repos/OpenOrbis/OpenOrbis-PS4-Toolchain/releases/latest -o rel.json
          grep browser_download_url rel.json
          URL=$(grep browser_download_url rel.json | grep -i 'tar.gz' | head -1 | cut -d '"' -f4)
          echo "Downloading: $URL"
          curl -L -o toolchain.tar.gz "$URL"
          mkdir -p toolchain
          tar -xzf toolchain.tar.gz -C toolchain
          find toolchain -name libkernel.h

      - name: Build ELF
        run: |
          H=$(find toolchain -path '*orbis/libkernel.h' | head -1)
          export OO_PS4_TOOLCHAIN=$PWD/$(dirname $(dirname $(dirname $H)))
          echo "Toolchain: $OO_PS4_TOOLCHAIN"
          make

      - name: Upload result
        uses: actions/upload-artifact@v4
        with:
          name: hello-build
          path: |
            *.elf
            *.o
