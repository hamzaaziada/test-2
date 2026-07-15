---
name: ma-one-pager
description: >
  Generate a standardized Significa Capital M&A company one-pager (single
  slide PPTX) in the canonical "acquisition target brief" format: purple
  left rail with fingerprint watermark and headline stats, white right
  panel with WHAT THEY DO, capability keyword pills, a 6-card mandate-fit
  grid, and a WHY IT FITS callout. Use whenever anyone asks to build,
  create, or generate a "one-pager", "target brief", "target profile",
  "company profile", or "acquisition brief" for an M&A target, prospect,
  or candidate — buy-side or sell-side, for any mandate. Triggers include
  "one-pager for X", "profile this company", "target brief for the
  [client] mandate", "do X like the AND Digital one".
---

# Significa Capital — M&A Company One-Pager

## Overview

This skill produces a **single-slide, 16:9 PPTX** company brief in the
canonical Significa Capital format. The reference deliverable is the
*AND Digital — LTIMindtree Acquisition Target Brief*; every one-pager
this skill produces must look like it came from the same analyst on the
same day: identical layout, identical palette, identical section order,
identical tone and level of detail. **Only the content changes per
company. Never redesign the layout, colors, fonts, or section order.**

The slide has two zones:

- **Left rail (dark purple)** — mandate tag, company name, tagline,
  3 headline stats, ownership strip, Significa fingerprint watermark.
- **Right panel (white)** — header (`Company | Sector`), WHAT THEY DO,
  CAPABILITY KEYWORDS pill row, `<CLIENT> MANDATE FIT` 3×2 card grid,
  WHY IT FITS callout, website link, confidentiality footer.

---

## Step 1 — Research the company

Gather every field in the Content Specification below before writing.
Prioritize sources in this order, and never present weaker-sourced facts
as stronger than they are:

1. **Company's own disclosures** — website, annual report, press releases.
2. **Statutory filings** — Companies House / SEC / local registries
   (revenue, founding year, ownership, directors).
3. **Reputable business press** — FT, Reuters, Bloomberg, major trade press.
4. **Professional networks** — LinkedIn (headcount ranges, leadership tenure).
5. **Databases/aggregators** (Crunchbase, PitchBook summaries) — use only
   to corroborate, and label figures from them as estimates.

Sourcing rules:

- Qualify figures honestly: `Est.`, `(reported)`, `~`, `900+` — exactly as
  the reference does (`USD 80M` / `Est. Revenue (reported)`).
- Client names may only be listed if publicly reported; introduce them in
  prose with "Reported clients include …".
- **If a data point cannot be found in reliable public sources, keep the
  section/field and use `Not publicly disclosed` as its value. Never
  delete a section, and never invent or extrapolate a figure.**
- Keep currency in the unit the source reports (USD/EUR/GBP), formatted
  as `USD 80M` style.

### Research prompt template

Use this to drive the research pass (web search or a research subagent):

> Research **[COMPANY]** ([website]) as a potential M&A target for the
> **[CLIENT]** mandate. Find, with sources: (1) one-line description of
> what they do and their sector; (2) latest revenue or a reported
> estimate, with currency and year; (3) headcount (or engineer count if
> a services firm); (4) founding year; (5) ownership structure —
> founder-led / PE-backed / listed — and the key leader's name and
> tenure; (6) publicly reported clients; (7) core service lines and
> technology capabilities; (8) HQ location and geographic footprint.
> Prefer the company's own site, statutory filings, and reputable press.
> For each figure, note the source and whether it is confirmed or an
> estimate. Mark anything unfindable as "Not publicly disclosed".

---

## Step 2 — Draft the content

Fill every field of the Content Specification, matching the reference's
voice and length exactly.

### House writing style (fixed)

Written by one hypothetical analyst, always:

- Third person, present tense, no first person, no questions.
- Clipped, evidence-dense sentences; sentence fragments are allowed and
  characteristic ("Founder-led by X with UK-wide presence.").
- Concrete nouns and numbers over adjectives. Allowed evaluative
  vocabulary: "strong track record", "proven", "strong alignment",
  "within range". Banned: hype ("world-class", "impressive", "exciting"),
  hedging ("might", "perhaps"), and marketing tone.
- Em dashes for appositions ("USD 80M — within range").
- No citations or footnotes on the slide itself; qualification is carried
  by `Est. / (reported) / ~` markers.

### Content Specification (field → rules → reference example)

| Field | Rules | Reference example |
|---|---|---|
| `mandateTag` | `<CLIENT> MANDATE`, ALL CAPS | `LTIMINDTREE MANDATE` |
| `companyName` | Official trading name | `AND Digital` |
| `tagline` | 4–8 words: geography + what they are | `UK Digital Engineering & Cloud Consultancy` |
| `stats` (exactly 3) | Short value + qualifier label. Convention: revenue, headcount, founded. Value ≤ 12 chars when possible; use `Not publicly disclosed` if unknown | `USD 80M` / `Est. Revenue (reported)`; `900+` / `Engineers`; `2014` / `Founded` |
| `ownershipLine` | 3 segments `A  \|  B  \|  C`: ownership model, key person, tenure | `Founder-Led  \|  Paramjit Uppal  \|  Since 2014` |
| `sectorLabel` | 2–4 words, the mandate lens on the company | `Data Engineering / Cloud` |
| `whatTheyDo` | 3–4 sentences, 45–60 words: (1) what the firm is and its service lines; (2) reported clients; (3) track record/differentiator; (4) ownership/leadership + footprint | see reference text in `scripts/generate_onepager.js` |
| `keywords` (5–7) | 1–3 word capability tags, Title Case | `Digital Engineering`, `Cloud Native`, `AWS`, … |
| `fitHeading` | `<CLIENT> MANDATE FIT` | `LTIMINDTREE MANDATE FIT` |
| `fitPoints` (exactly 6) | Card title = mandate criterion (1–3 words); evidence = ≤ 6 words of proof. Map to the client's actual stated criteria, not generic strengths. Use `Not publicly disclosed` as evidence when unknown | `Revenue` / `USD 80M — within range` |
| `whyItFits` | 3 sentences, 45–65 words: (1) ownership + strongest capability proof with named clients; (2) scale/delivery evidence with numbers; (3) explicit alignment to the mandate's priorities, ending on deal parameters | see reference text |
| `website` | Bare domain | `and.digital` |
| `footerDate` | `Month YYYY` of preparation | `June 2026` |

### Drafting prompt template

> Using the research notes below, draft the one-pager content for
> **[COMPANY]** under the **[CLIENT]** mandate. Produce every field of
> the Content Specification in `ma-one-pager/SKILL.md`, obeying the
> house style: third person, present tense, clipped evidence-dense
> sentences, no hype, qualify estimates with "Est./(reported)/~", and
> write "Not publicly disclosed" for anything missing. Match the
> reference lengths: WHAT THEY DO 3–4 sentences / 45–60 words; WHY IT
> FITS 3 sentences / 45–65 words; 5–7 keywords; exactly 3 stats and 6
> fit cards keyed to the mandate's stated criteria.

---

## Step 3 — Build the slide

1. Copy `scripts/generate_onepager.js` from this skill folder to a
   writable working directory.
2. Edit **only the `CONFIG` object** at the top with the drafted content.
   Leave `SVG_PATH` alone (it resolves to this skill's `assets/logos`
   relative to the script's original location) unless the script was
   moved somewhere that breaks the relative path — then set the
   `SIG_LOGO_PATH` env var to the absolute path of `assets/logos`.
3. Install the dependency if needed (`npm install pptxgenjs`), then run:

   ```bash
   node your_copy.js
   ```

   Output lands at `CONFIG.outputFileName`
   (override with the `ONEPAGER_OUT` env var).

## Step 4 — Visual QA (mandatory)

Convert and render, then look at the image:

```bash
soffice --headless --convert-to pdf <output>.pptx --outdir .
# render page 1 to PNG at 150dpi (pdftoppm, pymupdf, or equivalent)
```

If LibreOffice reports "source file could not be loaded", unzip the
`.pptx` and re-zip its contents with plain `zip -r` first, or install
`libreoffice-impress`.

Check:

- **WHAT THEY DO and WHY IT FITS fit their boxes** — if text overflows,
  shorten the copy (do not enlarge boxes or shrink fonts).
- Keyword pills wrap to a second row automatically; more than ~2 rows
  means the keywords are too long — trim to 5–7 short tags.
- Stat values aren't truncated (the script shrinks fonts for long values
  such as `Not publicly disclosed`, but verify).
- Ownership strip text fits on ≤ 2 lines.
- Fingerprint watermark is present, bleeding off the bottom-left, with
  the ownership strip readable on top of it.
- Exactly 6 fit cards, evidence lines on one line each.

Then deliver the `.pptx` (and the QA PDF/PNG if useful) to the user.

---

## Layout reference (fixed — do not modify)

Slide: 13.33" × 7.5" (`LAYOUT_WIDE`). All type is Arial.

**Left rail** — `#2F318E`, x = 0–3.85":
mandate tag (10pt bold, green, letterspaced) → company name (32pt bold
white; 26pt if > 16 chars) → tagline (13pt bold green) → hairline
`#3D3F7F` → three stats (28pt bold green value / 11pt `#AABBCC` label,
1.2" pitch) → ownership strip (rounded, `#1F6E5A`, 11pt bold white,
centered) → watermark: `Significa_Line_White.svg` at the bottom plus
oversized `Significa_Square_White.svg` fingerprint drawn over it, both
under the ownership strip.

**Right panel** — white, x = 4.05–13.08":
header `Company | Sector` (22pt; name bold `#2E3194`, sector `#A6A6A6`)
over a full-width `#458A6B` rule → section headings 10pt bold `#A6A6A6`
letterspaced → body copy 12pt `#434343` → lavender `#EEEDFE` keyword
pills (10.5pt bold `#2E3194`), auto-wrapping → grey hairline → 3×2 fit
grid (mint `#EDF7F0` rounded cards, green dot, 11.5pt bold `#1F8C6A`
title, 10pt `#595959` evidence) → WHY IT FITS callout at fixed y = 5.32"
(mint fill, `#458A6B` outline + left accent bar) → website line
(hyperlinked, purple, underlined) → footer 9.5pt `#A6A6A6`:
`Confidential — Internal Use Only  |  Significa Capital  |  <Month YYYY>`.

### Brand palette (fixed)

| Role | Hex |
|---|---|
| Left rail purple | `2F318E` |
| Header/pill/link purple | `2E3194` |
| Primary green (tag, stats, rule, dots, accents) | `458A6B` |
| Card-title green (fit titles, WHY IT FITS heading) | `1F8C6A` |
| Deep green (ownership strip) | `1F6E5A` |
| Stat labels on purple | `AABBCC` |
| Section headings / footer grey | `A6A6A6` |
| Body text | `434343` |
| Mint fill (cards, callout) | `EDF7F0` |
| Lavender fill (pills) | `EEEDFE` |

---

## Anti-patterns

1. Do not change layout, palette, fonts, section order, or headings
   (`WHAT THEY DO`, `CAPABILITY KEYWORDS`, `WHY IT FITS` are fixed).
2. Do not drop a section or stat because data is missing — write
   `Not publicly disclosed`.
3. Do not invent figures, clients, or capabilities; qualify estimates
   with `Est. / (reported) / ~`.
4. Do not write generic fit points — the 6 cards must map to the
   mandate's actual criteria (service lines, revenue range, geography,
   sector exposure, ownership).
5. Do not convert the SVG logos to PNG or substitute other logo files.
6. Do not skip visual QA — copy overflow in the two paragraph blocks is
   the most common defect.
7. Do not use this for multi-slide decks; it is a single-slide format.

## Files in this skill

- `SKILL.md` — this file.
- `scripts/generate_onepager.js` — the fixed-layout generator; its
  default `CONFIG` reproduces the AND Digital reference exactly (useful
  as a regression check and worked example).
- `assets/logos/` — Significa brand SVGs (watermark uses
  `Significa_Square_White.svg` + `Significa_Line_White.svg`).
