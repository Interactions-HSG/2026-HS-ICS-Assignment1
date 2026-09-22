# Toolchain, building, and the SD card

Everything you need on **your own laptop** to turn C code into something a Raspberry Pi can
boot. Come back here for Tasks 2, 3 and 4.

*(New here? Read the [README](../README.md) first — it covers getting your copy of the
repository.)*

---

## 1. Install the ARM toolchain (Task 2)

Your laptop has an Intel or Apple chip; the Raspberry Pi has an ARM chip. A normal compiler
would produce a program your laptop can run — useless here. You need a **cross-compiler**: one
that runs on your machine but produces code for a *different* processor.

Ours is called `arm-none-eabi-gcc`. The name says what it targets: **ARM** processor,
**none** = no operating system, **eabi** = embedded Application Binary Interface.

Download **version 13.3.Rel1** from
[developer.arm.com](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads):

| Your machine | File to download |
| --- | --- |
| Windows | `arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi.exe` |
| Mac, Apple silicon (M1–M4) | `arm-gnu-toolchain-13.3.rel1-darwin-arm64-arm-none-eabi.pkg` |
| Mac, Intel | `arm-gnu-toolchain-13.3.rel1-darwin-x86_64-arm-none-eabi.pkg` |

Not sure which Mac you have?  → **About This Mac**.

### Windows

Run the installer. On the **last page of the wizard, tick "Add path to environment variable"**.
If you miss it, run the installer again.

### macOS

Run the `.pkg`. Then tell your terminal where the tools are:

```bash
echo 'export PATH=/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/bin:$PATH' | tee -a ~/.zlogin && source ~/.zlogin
```

This appends one line to your Zsh configuration file, so the setting survives a restart.

### Check that it worked

```console
arm-none-eabi-gcc --version
arm-none-eabi-objcopy --version
```



On **Windows** the tools carry the version in their name:
`arm-none-eabi-gcc-13.3.1.exe --version`.

If you get **"command not found"**: the PATH step did not take. Open a **brand-new terminal
window** and try again — an already-open terminal does not pick up the change.

📸 **Task 2 asks for a screenshot** of the `arm-none-eabi-objcopy --version` output. Save it as
`screenshots/t1.png` in your repository, with your terminal prompt visible.

---

## 2. Prepare the SD card (once)

1. Format the SD card as **FAT32**.
2. Copy the three files from [`boot/`](../boot/) — `config.txt`, `fixup4.dat`, `start4.elf` —
   to the **top level** of the card. Do **not** make a `boot` folder on the card.

The Pi 400 boots from a small program in its EEPROM, which reads `config.txt`; `start4.elf`
and `fixup4.dat` are the minimal firmware it loads. Then it looks for a file called
**`kernel7l.img`** — that's your program.

---

## 3. Build your program

### The short way

```console
make power_blink     # Task 3 -- blinks the Pi's own power LED
make led_blink       # Task 4 -- blinks your LED on GPIO 16
make led_fade        # Bonus  -- fades your LED
```

Each produces **`kernel7l.img`** in the main folder of your repository. Copy that one file to
the SD card, eject the card properly, put it in the Pi, and plug in the power.

Only one program fits on the card at a time, so each build overwrites `kernel7l.img`. That is
on purpose.

On Windows, if `make` cannot find the compiler:

```console
make led_blink SUFFIX=-13.3.1.exe
```

Other targets: `make all` builds all three into `build/` without touching `kernel7l.img`;
`make toolchain` prints the tool versions; `make clean` deletes the build output.

### The long way

`make` just runs these two commands for you. First, compile the C code for the Pi 400's
Broadcom **BCM2711** chip:

```console
arm-none-eabi-gcc -g -nostartfiles -mfloat-abi=hard -O0 -DRPI4 \
    -mfpu=crypto-neon-fp-armv8 -march=armv8-a+crc -mcpu=cortex-a72 \
    power_blink.c -o kernel.elf
```

Roughly what those flags say: build for a 32-bit Cortex-A72 with hardware floating point
(`-mcpu`, `-march`, `-mfpu`, `-mfloat-abi`), do not optimise so the code stays readable
(`-O0`), keep debug information (`-g`), and **do not add the usual C start-up code**
(`-nostartfiles`) — there is no operating system here to set things up for us.

> You will see the warning `cannot find entry symbol _start`. That is expected for exactly
> that reason. Ignore it.

Second, strip away the ELF wrapper and leave the raw machine code the Pi's bootloader expects:

```console
arm-none-eabi-objcopy kernel.elf -O binary kernel7l.img
```

For Task 4 and the bonus, swap `power_blink.c` for `led_blink.c` or `led_fade.c`. Nothing else
changes.

---

## 4. Compiling inside a Codespace

The Codespace already has the toolchain, so `make led_blink` works there too. Use it to check
that your code **compiles** before you go near the hardware — it is much faster than the
edit → flash → boot loop.

It cannot do the rest: a Codespace has no SD card reader and no Raspberry Pi attached.

---

## 5. When something does not work

| Symptom | Usually means |
| --- | --- |
| `command not found: arm-none-eabi-gcc` | PATH not set, or terminal opened before you set it. Open a new one. |
| `command not found: make` | macOS: install Xcode command line tools (`xcode-select --install`). Windows: use the long-way commands instead. |
| `cannot find entry symbol _start` | Not an error. Expected. Ignore. |
| Pi does nothing, no LED at all | `kernel7l.img` missing or misnamed on the card, or the three `boot/` files are missing, or the card is not FAT32. |
| Power LED stays on solid | Your program ran but never toggles the pin — check which `GPFSEL`/`GPSET`/`GPCLR` register and bit you used. |
| External LED never lights | Check the wiring first (LED polarity — the long leg is +), then whether you set GPIO 16 to *output*. |
