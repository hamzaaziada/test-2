/**
 * Significa Capital — M&A Company One-Pager Generator
 *
 * Reproduces the canonical single-slide target brief format
 * (reference: "AND Digital — LTIMindtree Acquisition Target Brief").
 *
 * HOW TO USE:
 * 1. Copy this file somewhere writable.
 * 2. Edit ONLY the CONFIG object below with the target's details.
 * 3. Run: node your_copy.js
 * 4. Convert to PDF/images and visually QA (see SKILL.md).
 *
 * Do not change layout, coordinates, colors, or fonts below the CONFIG
 * block — this is the fixed house format. Only the content changes.
 */

const path = require("path");
const pptxgen = require("pptxgenjs");

// ============================================================
// CONFIG — EDIT THIS SECTION ONLY
// ============================================================
const CONFIG = {
  // Folder containing the Significa SVG logos. Defaults to the assets
  // folder that ships with this skill; override with SIG_LOGO_PATH env var.
  SVG_PATH:
    process.env.SIG_LOGO_PATH || path.join(__dirname, "..", "assets", "logos"),

  outputFileName: process.env.ONEPAGER_OUT || "AND_Digital_OnePager.pptx",

  // Left rail ---------------------------------------------------------
  mandateTag: "LTIMINDTREE MANDATE",          // ALL CAPS, "<CLIENT> MANDATE"
  companyName: "AND Digital",
  tagline: "UK Digital Engineering & Cloud Consultancy", // 4-8 word descriptor

  // Exactly 3 headline stats: short value + short qualifier label.
  // Use "Not publicly disclosed" as the value when unknown.
  stats: [
    { value: "USD 80M", label: "Est. Revenue (reported)" },
    { value: "900+", label: "Engineers" },
    { value: "2014", label: "Founded" },
  ],

  // Ownership/leadership strip (dark green box), "A  |  B  |  C" format
  ownershipLine: "Founder-Led  |  Paramjit Uppal  |  Since 2014",

  // Right panel --------------------------------------------------------
  sectorLabel: "Data Engineering / Cloud",    // header, after the "|"

  whatTheyDo:
    "UK digital engineering and consultancy firm offering product engineering, " +
    "cloud-native development, and data services. Reported clients include " +
    "Lloyds Banking Group, Nationwide, and Aviva. Strong track record in " +
    "cloud-native programme delivery on AWS, Azure, and GCP. Founder-led by " +
    "Paramjit Uppal with UK-wide and international presence.",

  // 5-7 short capability keywords (1-3 words each)
  keywords: [
    "Digital Engineering",
    "Cloud Native",
    "AWS",
    "Azure",
    "GCP",
    "Financial Services",
    "Product Engineering",
  ],

  fitHeading: "LTIMINDTREE MANDATE FIT",      // "<CLIENT> MANDATE FIT"

  // Exactly 6 fit cards: criterion title + evidence line (max ~6 words)
  fitPoints: [
    { title: "Data Engineering", evidence: "Primary service line" },
    { title: "Cloud", evidence: "AWS, Azure, GCP delivery" },
    { title: "Revenue", evidence: "USD 80M — within range" },
    { title: "Geography", evidence: "UK HQ — EU mandate match" },
    { title: "Financial Services", evidence: "Lloyds, Nationwide, Aviva" },
    { title: "Founder-Led", evidence: "Paramjit Uppal since 2014" },
  ],

  whyItFits:
    "Founder-led UK digital engineering firm with a proven track record " +
    "delivering cloud-native programmes for Lloyds Banking Group, Nationwide, " +
    "and Aviva. Multi-cloud delivery across AWS, Azure, and GCP with 900+ " +
    "engineers. Strong alignment with LTIMindtree's Data Engineering, Cloud, " +
    "and Financial Services priorities — and revenue within acquisition " +
    "parameters.",

  website: "and.digital",                     // bare domain, no protocol
  footerDate: "June 2026",                    // "Month YYYY" of preparation
};
// ============================================================
// END CONFIG — do not edit below this line
// ============================================================

const SVG = CONFIG.SVG_PATH;

// Fixed brand palette
const PURPLE_PANEL = "2F318E"; // left rail background
const PURPLE_TEXT = "2E3194"; // header name, pill text, website link
const PANEL_DIVIDER = "3D3F7F"; // hairline on the purple rail
const GREEN = "458A6B"; // tag, tagline, stat values, bar, dots, accents
const GREEN_TITLE = "1F8C6A"; // fit-card titles, WHY IT FITS heading
const GREEN_DEEP = "1F6E5A"; // ownership strip fill
const STEEL = "AABBCC"; // stat labels on purple
const GREY_HEAD = "A6A6A6"; // section headings, subtitle, footer
const BODY = "434343"; // body copy
const CARD_BODY = "595959"; // fit-card evidence lines
const MINT = "EDF7F0"; // fit cards + WHY IT FITS fill
const LAVENDER = "EEEDFE"; // keyword pill fill
const HAIRLINE = "E6E6E6"; // grey divider on white panel

const pres = new pptxgen();
pres.layout = "LAYOUT_WIDE"; // 13.33 x 7.5 in
pres.author = "Significa Capital";
pres.title = `${CONFIG.companyName} — Acquisition Target Brief`;

const slide = pres.addSlide();
slide.background = { color: "FFFFFF" };

// ----------------------------------------------------------------
// LEFT RAIL — dark purple
// ----------------------------------------------------------------
const PANEL_W = 3.85;
const LM = 0.25; // rail inner margin

slide.addShape(pres.shapes.RECTANGLE, {
  x: 0, y: 0, w: PANEL_W, h: 7.5,
  fill: { color: PURPLE_PANEL }, line: { type: "none" },
});

slide.addText(CONFIG.mandateTag.toUpperCase(), {
  x: LM, y: 0.24, w: 3.35, h: 0.3, fontFace: "Arial", fontSize: 10, bold: true,
  color: GREEN, charSpacing: 3, margin: 0,
});

slide.addText(CONFIG.companyName, {
  x: LM, y: 0.62, w: 3.4, h: 0.85, fontFace: "Arial",
  fontSize: CONFIG.companyName.length > 16 ? 26 : 32, bold: true,
  color: "FFFFFF", margin: 0, valign: "middle",
});

slide.addText(CONFIG.tagline, {
  x: LM, y: 1.52, w: 3.3, h: 0.65, fontFace: "Arial", fontSize: 13, bold: true,
  color: GREEN, margin: 0, valign: "top", lineSpacing: 17,
});

slide.addShape(pres.shapes.RECTANGLE, {
  x: LM, y: 2.14, w: 3.1, h: 0.015,
  fill: { color: PANEL_DIVIDER }, line: { type: "none" },
});

let statY = 2.3;
CONFIG.stats.slice(0, 3).forEach((s) => {
  slide.addText(s.value, {
    x: LM, y: statY, w: 3.4, h: 0.55, fontFace: "Arial",
    fontSize: s.value.length > 12 ? 18 : 28, bold: true,
    color: GREEN, margin: 0, valign: "middle",
  });
  slide.addText(s.label, {
    x: LM, y: statY + 0.6, w: 3.4, h: 0.3, fontFace: "Arial", fontSize: 11,
    color: STEEL, margin: 0, valign: "top",
  });
  statY += 1.2;
});

// Watermark, two layers (canonical look — bleeds off the panel on purpose):
// 1. Significa line logo (small mark + SIGNIFICA) near the bottom-left
// 2. Oversized fingerprint drawn over it; the Square logo's wordmark is
//    pushed below the slide edge so only the fingerprint shows.
// Both sit over the lower stats and under the ownership strip.
slide.addImage({
  path: `${SVG}/Significa_Line_White.svg`,
  x: 0.25, y: 6.76, w: 3.22, h: 3.22 * (190 / 908),
});
slide.addImage({
  path: `${SVG}/Significa_Square_White.svg`,
  x: -1.51, y: 4.9, w: 7.24, h: 7.24 * (382 / 450),
});

slide.addShape(pres.shapes.ROUNDED_RECTANGLE, {
  x: LM, y: 5.86, w: 3.35, h: 0.62, rectRadius: 0.07,
  fill: { color: GREEN_DEEP }, line: { type: "none" },
});
slide.addText(CONFIG.ownershipLine, {
  x: LM + 0.1, y: 5.86, w: 3.15, h: 0.62, fontFace: "Arial", fontSize: 11,
  bold: true, color: "FFFFFF", align: "center", valign: "middle",
  margin: 0, lineSpacing: 14,
});

// ----------------------------------------------------------------
// RIGHT PANEL — white
// ----------------------------------------------------------------
const RX = 4.05;
const RW = 9.03; // right edge at 13.08 (0.25 outer margin)

// Header: "<Company>  |  <Sector>" with green rule beneath
slide.addText(
  [
    { text: CONFIG.companyName, options: { bold: true, color: PURPLE_TEXT } },
    { text: "  |  ", options: { color: "C9C9C9" } },
    { text: CONFIG.sectorLabel, options: { color: GREY_HEAD } },
  ],
  { x: RX, y: 0.26, w: RW, h: 0.5, fontFace: "Arial", fontSize: 22, margin: 0, valign: "middle" }
);
slide.addShape(pres.shapes.RECTANGLE, {
  x: RX, y: 0.66, w: RW, h: 0.035,
  fill: { color: GREEN }, line: { type: "none" },
});

// WHAT THEY DO
slide.addText("WHAT THEY DO", {
  x: RX, y: 0.82, w: RW, h: 0.24, fontFace: "Arial", fontSize: 10, bold: true,
  color: GREY_HEAD, charSpacing: 3, margin: 0,
});
slide.addText(CONFIG.whatTheyDo, {
  x: RX, y: 1.06, w: RW, h: 1.0, fontFace: "Arial", fontSize: 12,
  color: BODY, margin: 0, valign: "top", lineSpacing: 17,
});

// CAPABILITY KEYWORDS — lavender pills, auto-wrapping
slide.addText("CAPABILITY KEYWORDS", {
  x: RX, y: 2.12, w: RW, h: 0.24, fontFace: "Arial", fontSize: 10, bold: true,
  color: GREY_HEAD, charSpacing: 3, margin: 0,
});
let kx = RX;
let ky = 2.38;
CONFIG.keywords.forEach((k) => {
  const kw = k.length * 0.085 + 0.32;
  if (kx + kw > RX + RW) { kx = RX; ky += 0.38; } // wrap, never overflow
  slide.addShape(pres.shapes.ROUNDED_RECTANGLE, {
    x: kx, y: ky, w: kw, h: 0.26, rectRadius: 0.04,
    fill: { color: LAVENDER }, line: { type: "none" },
  });
  slide.addText(k, {
    x: kx, y: ky, w: kw, h: 0.26, fontFace: "Arial", fontSize: 10.5, bold: true,
    color: PURPLE_TEXT, align: "center", valign: "middle", margin: 0,
  });
  kx += kw + 0.13;
});
const pillsBottom = ky + 0.26;

slide.addShape(pres.shapes.RECTANGLE, {
  x: RX, y: pillsBottom + 0.16, w: RW, h: 0.02,
  fill: { color: HAIRLINE }, line: { type: "none" },
});

// MANDATE FIT — 3 x 2 grid of mint cards
const fitHeadY = pillsBottom + 0.28;
slide.addText(CONFIG.fitHeading.toUpperCase(), {
  x: RX, y: fitHeadY, w: RW, h: 0.24, fontFace: "Arial", fontSize: 10,
  bold: true, color: GREY_HEAD, charSpacing: 3, margin: 0,
});
const COL_W = (RW - 2 * 0.15) / 3;
const ROW_H = 0.62;
const gridY = fitHeadY + 0.32;
CONFIG.fitPoints.slice(0, 6).forEach((p, i) => {
  const cx = RX + (i % 3) * (COL_W + 0.15);
  const cy = gridY + Math.floor(i / 3) * (ROW_H + 0.13);
  slide.addShape(pres.shapes.ROUNDED_RECTANGLE, {
    x: cx, y: cy, w: COL_W, h: ROW_H, rectRadius: 0.05,
    fill: { color: MINT }, line: { type: "none" },
  });
  slide.addShape(pres.shapes.OVAL, {
    x: cx + 0.17, y: cy + 0.18, w: 0.13, h: 0.13,
    fill: { color: GREEN }, line: { type: "none" },
  });
  slide.addText(p.title, {
    x: cx + 0.42, y: cy + 0.06, w: COL_W - 0.5, h: 0.26, fontFace: "Arial",
    fontSize: 11.5, bold: true, color: GREEN_TITLE, margin: 0, valign: "middle",
  });
  slide.addText(p.evidence, {
    x: cx + 0.42, y: cy + 0.33, w: COL_W - 0.5, h: 0.24, fontFace: "Arial",
    fontSize: 10, color: CARD_BODY, margin: 0, valign: "top",
  });
});

// WHY IT FITS — mint callout with green accent bar (fixed position)
const whyY = 5.32;
const WHY_H = 1.28;
slide.addShape(pres.shapes.RECTANGLE, {
  x: RX, y: whyY, w: RW, h: WHY_H,
  fill: { color: MINT }, line: { color: GREEN, width: 0.75 },
});
slide.addShape(pres.shapes.RECTANGLE, {
  x: RX, y: whyY, w: 0.06, h: WHY_H,
  fill: { color: GREEN }, line: { type: "none" },
});
slide.addText("WHY IT FITS", {
  x: RX + 0.27, y: whyY + 0.1, w: RW - 0.55, h: 0.24, fontFace: "Arial",
  fontSize: 10, bold: true, color: GREEN_TITLE, charSpacing: 3, margin: 0,
});
slide.addText(CONFIG.whyItFits, {
  x: RX + 0.27, y: whyY + 0.36, w: RW - 0.55, h: WHY_H - 0.46,
  fontFace: "Arial", fontSize: 12, color: BODY, margin: 0, valign: "top",
  lineSpacing: 16,
});

// Website + footer
slide.addText(
  [
    { text: "Website:  ", options: { bold: true, color: GREY_HEAD } },
    {
      text: CONFIG.website,
      options: {
        bold: true, color: PURPLE_TEXT, underline: { style: "sng" },
        hyperlink: { url: `https://${CONFIG.website.replace(/^https?:\/\//, "")}` },
      },
    },
  ],
  { x: RX, y: 6.72, w: RW, h: 0.3, fontFace: "Arial", fontSize: 12, margin: 0 }
);

slide.addText(
  `Confidential — Internal Use Only  |  Significa Capital  |  ${CONFIG.footerDate}`,
  {
    x: RX, y: 7.18, w: RW, h: 0.25, fontFace: "Arial", fontSize: 9.5,
    color: GREY_HEAD, margin: 0,
  }
);

pres
  .writeFile({ fileName: CONFIG.outputFileName })
  .then(() => console.log("done: " + CONFIG.outputFileName));
