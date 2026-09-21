#!/usr/bin/env python3
"""Completeness check for an Assignment 1 hand-in.

This does NOT grade anything and never looks at whether an answer is *correct* --
it only tells the student which parts of the hand-in are still missing, so nobody
loses points to an oversight. Run it locally with:

    python3 .github/scripts/check_submission.py
"""
from __future__ import annotations

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]

# `TODO:` at the start of a line or at the start of a markdown table cell,
# ignoring any occurrence inside an inline `code span`.
CODE_SPAN = re.compile(r"`[^`]*`")
TODO = re.compile(r"(?:^|\|)\s*TODO:")
HEADING = re.compile(r"^(#{1,6})\s+(.*)$")


class Report:
    def __init__(self) -> None:
        self.problems: list[tuple[str, str]] = []
        self.notes: list[str] = []

    def fail(self, what: str, detail: str) -> None:
        self.problems.append((what, detail))

    def note(self, msg: str) -> None:
        self.notes.append(msg)

    @property
    def ok(self) -> bool:
        return not self.problems


def find_todos(text: str) -> list[tuple[int, str]]:
    """Return (line number, enclosing heading) for every unfilled slot."""
    hits: list[tuple[int, str]] = []
    heading = "(top of file)"
    for n, raw in enumerate(text.splitlines(), start=1):
        m = HEADING.match(raw.strip())
        if m:
            heading = m.group(2).strip()
            continue
        if TODO.search(CODE_SPAN.sub("", raw)):
            hits.append((n, heading))
    return hits


def section_body(text: str, heading_substring: str) -> str:
    """Text between the heading containing `heading_substring` and the next heading."""
    lines = text.splitlines()
    start = None
    level = 0
    for i, raw in enumerate(lines):
        m = HEADING.match(raw.strip())
        if m and heading_substring.lower() in m.group(2).lower():
            start, level = i + 1, len(m.group(1))
            break
    if start is None:
        return ""
    out = []
    for raw in lines[start:]:
        m = HEADING.match(raw.strip())
        if m and len(m.group(1)) <= level:
            break
        out.append(raw)
    return "\n".join(out)


def check_report(r: Report) -> None:
    path = ROOT / "REPORT.md"
    if not path.exists():
        r.fail("REPORT.md", "File is missing -- it must not be renamed or deleted.")
        return

    text = path.read_text(encoding="utf-8")
    todos = find_todos(text)
    if todos:
        by_section: dict[str, list[int]] = {}
        for line, heading in todos:
            by_section.setdefault(heading, []).append(line)
        detail = "\n".join(
            f"  - **{heading}** — line(s) {', '.join(str(n) for n in lines)}"
            for heading, lines in by_section.items()
        )
        r.fail(
            "REPORT.md",
            f"{len(todos)} unanswered slot(s) still contain `TODO:`:\n{detail}",
        )

    team = section_body(text, "Team & process")
    if team and re.search(r"\|\s*Team member 1[^|]*\|\s*(TODO:)?\s*\|", team):
        r.fail("REPORT.md", "Team member 1 is empty -- fill in at least one name.")


def check_screenshot(r: Report) -> None:
    path = ROOT / "screenshots" / "t1.png"
    if not path.exists():
        r.fail(
            "screenshots/t1.png",
            "Task 2 screenshot is missing. Save your local "
            "`arm-none-eabi-objcopy --version` output as `screenshots/t1.png`.",
        )
        return
    if path.read_bytes()[:8] != b"\x89PNG\r\n\x1a\n":
        r.fail(
            "screenshots/t1.png",
            "File is not a real PNG. Renaming a .jpg to .png does not convert it -- "
            "export it as PNG instead.",
        )


def check_code(r: Report) -> None:
    def body(name: str) -> str | None:
        p = ROOT / name
        if not p.exists():
            return None
        # Ignore comments and whitespace so leftover TODO comments don't count as work.
        src = p.read_text(encoding="utf-8")
        src = re.sub(r"/\*.*?\*/", "", src, flags=re.S)
        src = re.sub(r"//[^\n]*", "", src)
        return re.sub(r"\s+", "", src)

    power, blink, fade = body("power_blink.c"), body("led_blink.c"), body("led_fade.c")

    if blink is None:
        r.fail("led_blink.c", "File is missing -- Task 4 needs it.")
    elif power is not None and blink == power:
        r.fail(
            "led_blink.c",
            "Still identical to `power_blink.c`. Task 4 asks you to drive GPIO 16, "
            "not the power LED on GPIO 42.",
        )

    if fade is None:
        r.note("`led_fade.c` is absent — the bonus task is optional.")
    else:
        attempt = section_body(
            (ROOT / "REPORT.md").read_text(encoding="utf-8"), "Which approach did you choose"
        ) if (ROOT / "REPORT.md").exists() else ""
        declined = "not attempted" in attempt.lower()
        untouched = "while(1){;}" in fade.replace(" ", "")
        if untouched and not declined:
            r.note(
                "`led_fade.c` looks untouched. That is fine — the bonus is optional — but "
                "write `not attempted` in section 5.1 so we know it was deliberate."
            )


def main() -> int:
    r = Report()
    check_report(r)
    check_screenshot(r)
    check_code(r)

    lines: list[str] = ["# Assignment 1 — submission check", ""]
    if r.ok:
        lines += [
            "✅ **Your hand-in looks complete.**", "",
            "This only checks that nothing is *missing* — it says nothing about whether "
            "your answers are right.", "",
            "Remember to add the grader account as a collaborator and submit your "
            "repository URL on Canvas.",
        ]
    else:
        lines += [f"❌ **{len(r.problems)} thing(s) still to do.**", ""]
        for what, detail in r.problems:
            lines += [f"### `{what}`", "", detail, ""]

    if r.notes:
        lines += ["---", "", "### Notes", ""] + [f"- {n}" for n in r.notes]

    out = "\n".join(lines)
    print(out)
    summary = os.environ.get("GITHUB_STEP_SUMMARY")
    if summary:
        Path(summary).write_text(out, encoding="utf-8")
    return 0 if r.ok else 1


if __name__ == "__main__":
    sys.exit(main())
