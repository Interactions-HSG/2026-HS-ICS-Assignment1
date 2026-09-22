# Assignment 1: Bare Metal Raspberry Pi

**Einführung in Computersysteme — HS 2026 · 10 points · Deadline: 11 October 2026, 23:59 CET**

You will make a Raspberry Pi blink an LED with **no operating system** — just your code and
the hardware. 📄 The questions and all the detail are in
**[`docs/assignment1.pdf`](docs/assignment1.pdf)**. This page is only about *how to work and
hand in*.

**You need:** a Raspberry Pi 400 PC-Kit, a 330 Ω resistor, an LED, a breadboard, and two
male-to-female jumper wires.

**You work in your assigned group of two.** You share **one** repository: one of you creates
it, the other joins as a collaborator. Only one of you hands in.

**Never used Git or GitHub?** That's expected. Follow the six steps below in order and you
will be fine. It takes about 10 minutes.

*Original repository: <https://github.com/Interactions-HSG/2026-HS-ICS-Assignment1>*

---

## Step 1 · One of you makes the copy

> **Decide who does this — only one of you.** If your partner already created the repository,
> skip to Step 3 and ask them to add you.

On the [assignment repository](https://github.com/Interactions-HSG/2026-HS-ICS-Assignment1),
click the green **`Use this template`** → **`Create a new repository`**.

Then:

| Setting | What to choose |
| --- | --- |
| Repository name | `ics-a1-group<your number>`, e.g. **`ics-a1-group7`** |
| Visibility | **Private** ← important |

Your **group number** is the one from the group assignment on Canvas. Use digits only, no
spaces: `ics-a1-group7`, not `ics-a1-Group 7` or `ics-a1-gruppe7`. We match repositories to
groups by that name, so getting it right saves everyone time.

Click **Create repository**. Your group now has its repository at
`github.com/your-username/ics-a1-group7`.

> **Why not "Fork"?**
> You may have seen a **Fork** button on GitHub. Forking also copies a project, but a fork of
> a public project is **always public** — everyone in the course could read your answers and
> your Bandit passwords. **Use this template** gives you a private copy instead. So: template,
> not fork.

## Step 2 · Add your partner and us

The repository is private, so right now *nobody* can see it — not your partner, not us. Whoever
created it goes to:

**Settings** → **Collaborators** → **Add people**, and adds **three** people:

```
<your partner's GitHub username>
lukabekavac
Karimkh31
```

Your partner then gets an email invitation and has to **accept** it before they can push.

> ⚠️ **Forget this and we cannot open your repository, so we cannot grade you.** Do it now, not
> on the deadline.

## Step 3 · Open your repository

You have two options. **Codespaces is the easy one** — start there.

### Option A — Codespaces (recommended)

In **your** repository: **`Code ▾`** → **`Codespaces`** → **`Create codespace on main`**.

> **What is a Codespace?** A complete Linux computer running in your browser: a code editor, a
> terminal, and all the tools for this assignment already installed. Nothing to install on your
> laptop. Your files live in your repository, so you can close the tab and come back later.
> GitHub gives every account a free monthly allowance, far more than this assignment needs.

The first start takes 2–3 minutes while the compiler downloads. After that it opens in seconds.

💡 When you are done for the day, close the Codespace (**`Code ▾` → `Codespaces` → `...` →
`Stop codespace`**) so it does not use up your free hours.

### Option B — On your own computer

Install [Git](https://git-scm.com/downloads) if you do not have it, then in a terminal:

```console
git clone https://github.com/your-username/ics-a1-group7.git
cd ics-a1-group7
```

`clone` means "download this repository onto my computer, and remember where it came from".
Use the URL of **your group's** repository, not the one you are reading now.

## Step 4 · Answer the questions

Open **[`REPORT.md`](REPORT.md)**. That file *is* your hand-in — there is no zip file and
nothing to upload.

Every place that needs an answer from you holds a line that starts with `TODO:`. Replace that
whole line with your answer. That's the entire trick.

## Step 5 · Save your work (this is what Git is for)

Editing a file is not enough — you have to **commit** it (take a snapshot) and **push** it
(upload that snapshot to GitHub). Until you push, we see nothing.

**In Codespaces or VS Code**, use the **Source Control** panel on the left (the branching icon):
type a short message, click **Commit**, then click **Sync Changes**. That's it.

**In a terminal**, the same thing in three commands:

```console
git add .                        # which changes go into the snapshot -- "." means all of them
git commit -m "Answered task 3"  # take the snapshot, with a short note about what you did
git push                         # upload it to GitHub
```

Do this often — after every task, not once at the end. It costs 10 seconds and it means a
crashed browser cannot eat your evening.

> **Working with your partner:** you share one repository, so run `git pull` (or click
> **Sync Changes**) *before* you start working, to get their latest changes. If you both edit
> the same lines at the same time, Git will ask you to sort out the conflict — easiest to avoid
> by agreeing who does which task.

**Check it worked:** open your repository on github.com and look at `REPORT.md`. If your
answers are there, you are safe.

### You get automatic feedback

Every time you push, a check runs and tells you what is still missing: unanswered questions, a
missing screenshot, C code that does not compile. Click the **Actions** tab of your repository
to see it — a green ✓ means nothing is missing, a red ✗ tells you exactly what to fix.

It checks **completeness, not correctness**. Green does not mean your answers are right.

## Step 6 · Hand in

1. No `TODO:` lines left in `REPORT.md` — including your **group number** and **both names**
   at the top
2. Everything **pushed** (Step 5)
3. Your partner, `lukabekavac` and `Karimkh31` added as collaborators (Step 2)
4. **One of you** — not both — submits **only the repository URL** on Canvas:
   `https://github.com/your-username/ics-a1-group7`

We grade whatever is on your `main` branch at the deadline. Both of you get the same mark.

---

## One thing Codespaces cannot do

A Codespace lives in a data centre. It cannot reach your SD card reader and it cannot power
your Raspberry Pi. So for Tasks 2, 3 and 4 you also need the ARM compiler **on your own
laptop** — that is Task 2 of the assignment.

👉 **[`docs/TOOLCHAIN.md`](docs/TOOLCHAIN.md)** has the install instructions, the build
commands, and how to prepare the SD card.

Use the Codespace to write and to check that your C code compiles; use your own machine to put
`kernel7l.img` on the card and run it on the Pi.

## What's in this repository

| File | What it is |
| --- | --- |
| **`REPORT.md`** | **Your hand-in.** Answer the questions here. |
| `power_blink.c` | Given to you. Blinks the Pi's power LED. Read it, don't change it. |
| `led_blink.c` | **Task 4.** Starts as a copy of `power_blink.c` — make it drive GPIO 16. |
| `led_fade.c` | **Bonus task.** Starting point for fading the LED. |
| `boot/` | Firmware files to copy onto your SD card. |
| `screenshots/` | Put your `t1.png` here. |
| `docs/` | The assignment sheet, the toolchain guide, and the BCM2711 manual. |
| `Makefile` | Build shortcuts: `make led_blink` and friends. |

## Stuck?

- The **[BCM2711 ARM Peripherals manual](docs/bcm2711-peripherals.pdf)** — pages **65–70**
  (GPIO registers) and **4–6** (addresses) are what this assignment is really about.
- [pinout.xyz](https://pinout.xyz/) — which physical pin is which GPIO number.
- The [valvers.com bare-metal tutorial](https://www.valvers.com/open-software/raspberry-pi/bare-metal-programming-in-c-part-1/)
  this assignment follows. It solves a *similar* problem, not yours — you will have to adapt
  it. Skip its "Generating SD Cards" part; `boot/` already has those files.
- **Using AI is encouraged** — but ask *"explain how to solve this"*, not *"solve this for me"*.
  The skill you are building is reading a hardware manual yourself. Write down what you used in
  section 0.4 of the report.
- Come to the exercise session, or ask in your repository's **Issues** tab and mention
  `@lukabekavac` or `@Karimkh31`.

## Contact

- Luka Bekavac — <luka.bekavac@unisg.ch> · GitHub `lukabekavac`
- Karim Khamaisi — <karim.khamaisi@unisg.ch> · GitHub `Karimkh31`
- Simon Mayer — <simon.mayer@unisg.ch>

## License

MIT — see [`LICENSE`](LICENSE).
