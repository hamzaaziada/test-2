# Meshpool — front end

A ground-up redesign of the [meshpool.ai](https://meshpool.ai) landing page.

**Meshpool** pools the GPUs you already have — cloud instances, on‑prem servers,
workstations — into a single OpenAI‑compatible API. This site tells that story
with the polish of an Apple / Microsoft / Rippling product page, and loads fast
enough to prove the "make it faster" point on its own.

## Design principles

- **Native type, zero font downloads.** The system font stack (SF Pro on Apple
  platforms, Segoe UI on Windows) gives the page a first-party feel and removes
  the single largest render-blocking asset most landing pages ship.
- **One request.** Everything — CSS, JS, icons, the animated mesh diagram, even
  the favicon — is inlined. No frameworks, no CDNs, no build step.
- **Restraint over decoration.** One accent color, a strict type scale with
  tight display tracking, generous whitespace, hairline borders, and pill
  buttons. Motion is limited to a subtle scroll reveal and the data-flow
  animation in the hero mesh — both disabled under `prefers-reduced-motion`.
- **Dark mode for free.** The entire palette is design tokens; `prefers-color-scheme`
  swaps them in a media query.
- **Accessible.** Semantic landmarks, labeled navigation, `aria` on decorative
  SVG, visible-text contrast at AA or better in both themes.

## Structure

```
index.html   the entire site (markup + inline CSS + ~20 lines of vanilla JS)
```

## Develop

Open `index.html` in a browser. That's it — there is intentionally nothing to
install, build, or deploy beyond copying one file to any static host.
