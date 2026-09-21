# Assignment 1: Bare Metal Raspberry Pi (10 pt)

**Einführung in Computersysteme, HS 2026 — Institute of Computer Science, University of St.Gallen**

**Deadline: 11 October 2026, 23:59 CET**

In this assignment you set up a "bare metal" development environment for the Raspberry Pi —
no operating system, just your code talking straight to the hardware — and you write, compile
and run a few small programs on real silicon.

You will need: a **Raspberry Pi 400 PC-Kit**, a **330 Ω resistor**, an **LED**, a
**breadboard**, and two **male-to-female jumper wires**.

> 📄 The full assignment sheet, with the wiring diagram and all the detail, is
> **[`docs/ICS_Assignment_1__Bare_Metal.pdf`](docs/ICS_Assignment_1__Bare_Metal.pdf)**.
> This README tells you how to *work* on it; the PDF tells you what the tasks *are*.

---

## 1. Get your own copy

Do **not** fork this repository — a fork of a public repo is always public, and your answers
would be readable by everyone in the course.

1. Click the green **`Use this template`** button at the top of this page →
   **`Create a new repository`**.
2. Name it `ics-a1-<yourlastname>` (add your team-mates' names if you work in a team).
3. Set the visibility to **Private**. ⚠️ This matters.
4. Create the repository.
5. **Settings → Collaborators → Add people** → add your grader
   (you will find the account name on Canvas). Without this we cannot open your repo and
   cannot grade you.
6. If you work in a team, add your team-mates as collaborators too, so you can all push.

## 2. Open it in a Codespace

On *your* new repository: **`Code` ▾ → `Codespaces` → `Create codespace on main`**.

You get a full Linux machine in the browser with the ARM cross-compiler already installed.
The first start takes two or three minutes while the toolchain downloads.

Use the Codespace to **write your answers** and to **compile and sanity-check your C code**.

> ⚠️ **You still need a local setup.** A Codespace cannot reach your SD card reader and it
> cannot power your Raspberry Pi. Task 2 also explicitly asks you to install the toolchain on
> *your own machine*. So: think in the Codespace, but flash and test on your own hardware.

You can of course also just `git clone` the repo and work entirely locally. Nothing here
depends on Codespaces.

## 3. Write your answers in `REPORT.md`

**[`REPORT.md`](REPORT.md) is your hand-in.** There is no zip file and no `report.txt` any
more — you edit that one Markdown file directly and commit it.

Every spot that needs an answer from you holds a line starting with `TODO:`. Replace the
whole line. That's it.

Commit and push as you go — `git commit` early and often is a habit worth building, and it
also means you cannot lose work to a browser crash the night before the deadline.

### Automatic feedback

Every time you push, a **Submission check** runs in the **Actions** tab and tells you what is
still missing: unanswered questions, a missing screenshot, code that does not compile.

It checks **completeness, not correctness** — a green tick means you handed in everything, not
that your answers are right.

You can run exactly the same check yourself at any time:

```console
$ python3 .github/scripts/check_submission.py
```

---

## 4. Set up the toolchain on your own machine

You need the **ARM GNU toolchain, version 13.3.Rel1** — the `arm-none-eabi-*` tools
(ARM processor / **n**o **o**perating system / **e**mbedded **ABI**), for 32-bit targets.
Download it from [developer.arm.com](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads):

| Platform | Installer |
| --- | --- |
| Windows | `arm-gnu-toolchain-13.3.rel1-mingw-w64-i686-arm-none-eabi.exe` |
| macOS (Intel) | `arm-gnu-toolchain-13.3.rel1-darwin-x86_64-arm-none-eabi.pkg` |
| macOS (Apple silicon) | `arm-gnu-toolchain-13.3.rel1-darwin-arm64-arm-none-eabi.pkg` |

On **Windows**, tick the box at the end of the installer wizard that adds the toolchain to
your `PATH`.

On **macOS**, the binaries land in
`/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/bin` and you have to add them to your
`PATH` yourself:

```console
% echo 'export PATH=/Applications/ArmGNUToolchain/13.3.rel1/arm-none-eabi/bin:$PATH' | tee -a ~/.zlogin && source ~/.zlogin
```

That appends a single line to your Zsh configuration file, so the setting survives a restart.

### Check that it worked

```console
% arm-none-eabi-gcc --version
% arm-none-eabi-objcopy --version
```

On **Windows** the executables carry the version in their name, so use
`arm-none-eabi-gcc-13.3.1.exe --version` and
`arm-none-eabi-objcopy-13.3.1.exe --version` instead.

If you get "command not found", the `PATH` step did not take effect — open a **new** terminal
window and try again.

📸 **Task 2 wants a screenshot** of the `arm-none-eabi-objcopy --version` output, saved as
[`screenshots/t1.png`](screenshots/).

---

## 5. Build and deploy

### The short way

```console
$ make power_blink     # Task 3 -- blinks the Pi's power LED
$ make led_blink       # Task 4 -- blinks your LED on GPIO 16
$ make led_fade        # Bonus  -- fades your LED
```

Each one produces **`kernel7l.img`** in the repository root. That single file is what goes on
the SD card. Only one program can live on the card at a time, so each build overwrites it —
that is intentional.

On Windows, if `make` cannot find the compiler:

```console
$ make led_blink SUFFIX=-13.3.1.exe
```

Other useful targets: `make all` builds all three into `build/` without touching
`kernel7l.img`, `make toolchain` prints the tool versions, `make clean` removes the artifacts.

### The long way

The `Makefile` is only a convenience — here is what it actually runs. First cross-compile the
C source for the Pi 400's Broadcom **BCM2711** (a 32-bit Cortex-A72 target):

```console
$ arm-none-eabi-gcc -g -nostartfiles -mfloat-abi=hard -O0 -DRPI4 \
    -mfpu=crypto-neon-fp-armv8 -march=armv8-a+crc -mcpu=cortex-a72 \
    power_blink.c -o kernel.elf
```

> The linker warning `cannot find entry symbol _start` is expected — there is no C runtime
> here. Ignore it.

Then strip the ELF container down to a raw binary image the Pi's bootloader can execute:

```console
$ arm-none-eabi-objcopy kernel.elf -O binary kernel7l.img
```

For Task 4 and the bonus, swap `power_blink.c` for `led_blink.c` or `led_fade.c`. Nothing
else changes.

### Preparing the SD card

Do this once:

1. Format the SD card as **FAT32**.
2. Copy the three files from [`boot/`](boot/) — `config.txt`, `fixup4.dat`, `start4.elf` — to
   the **root** of the card. Do *not* create a `boot/` folder on the card.

Then, for every build: copy `kernel7l.img` to the root of the card, eject it properly, put it
in the Pi, and plug in the power adapter.

The Pi 400 boots from an EEPROM bootloader configured by `config.txt`; `fixup4.dat` and
`start4.elf` are the minimal GPU firmware it needs. `kernel7l.img` is the filename the
bootloader looks for on this model — hence the name.

---

## 6. What's in this repository

| Path | What it is |
| --- | --- |
| **`REPORT.md`** | **Your hand-in.** Answer the questions here. |
| `power_blink.c` | Given to you. Blinks the Pi's power LED (GPIO 42). Read it, don't change it. |
| `led_blink.c` | **Task 4.** Starts as a copy of `power_blink.c`; make it drive GPIO 16. |
| `led_fade.c` | **Bonus.** Starting point for fading the LED. |
| `boot/` | Firmware files to copy onto the SD card. |
| `screenshots/` | Put `t1.png` here. |
| `docs/` | The assignment sheet and the BCM2711 ARM Peripherals manual. |
| `Makefile` | Build shortcuts. |
| `.devcontainer/` | Codespaces setup — installs the cross-compiler. |
| `.github/` | The automatic submission check. |

---

## 7. Submitting

1. Everything answered in `REPORT.md`, `screenshots/t1.png` in place, code committed.
2. **Push.** Anything not pushed does not exist as far as we are concerned.
3. Check the **Actions** tab is green.
4. Grader account added as a collaborator (step 1.5 above).
5. Submit **only your repository URL** on Canvas —
   `https://github.com/<you>/ics-a1-<yourname>`.

We grade the state of the `main` branch at the deadline.

---

## Using AI

You are **encouraged** to use AI tools here — but ask *"explain how to solve this"*, not
*"solve this for me"*. The point of the assignment is that you end up able to read a hardware
manual and reason about registers yourself, which is a skill an LLM cannot hand you. Note down
what you used in section 0.4 of the report.

## Where to look when you get stuck

- The **[BCM2711 ARM Peripherals manual](docs/bcm2711-peripherals.pdf)** — pages **65–70**
  (GPIO registers) and **4–6** (address mapping) are what this assignment is about.
- [pinout.xyz](https://pinout.xyz/) — which physical pin is which GPIO.
- The [valvers.com bare-metal tutorial](https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-1/)
  that this assignment follows. It solves a *similar* problem, not yours — you will have to
  adapt it. You can skip its "Generating SD cards" part; `boot/` already has what you need.
- Come to the exercise session, or open an issue in your own repository and tag your grader.

---

## License

Distributed under the MIT License. See [`LICENSE`](LICENSE).

## Contact

- Luka Bekavac — <luka.bekavac@unisg.ch>
- Karim Khamaisi — <karim.khamaisi@unisg.ch>
- Simon Mayer — <simon.mayer@unisg.ch>
