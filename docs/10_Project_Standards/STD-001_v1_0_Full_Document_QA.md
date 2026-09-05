# STD-001 v1.0 — Full-document QA

**Date:** 2026-09-05
**Branch:** `docs/pre-code-foundation-plan`
**Result:** PASS

## Published artifacts

- `STD-001_Glossary_RU_v1_0_Current.docx`
- `STD-001_Glossary_DE_v1_0_Current.docx`
- Matching RU and DE editable Markdown sources are retained next to the Word pair.

## Content verification

| Check | RU | DE | Result |
|---|---:|---:|---|
| Rendered pages | 27 | 26 | PASS |
| Body paragraphs | 139 | 139 | PASS |
| Tables | 29 | 29 | PASS |
| Table rows | 328 | 328 | PASS |
| Semantic headings | 33 | 33 | PASS |
| Tables with repeated semantic header row | 29 / 29 | 29 / 29 | PASS |
| Required canonical tokens | Present | Present | PASS |

The RU and DE documents have identical structure and normative strength. The one-page rendering difference is caused by natural language expansion and does not change sections, tables, rows, tokens or requirements.

## Coverage

The glossary defines the canonical vocabulary for document and implementation status, architecture, modules, events, persistence, save/restore, configuration, game adapters, Memory Engine, AI, tests, security, operations, versioning, Git, risks, time and abbreviations. It also records forbidden ambiguities, translation rules, new-term admission and acceptance criteria. More specific accepted ADRs and contracts retain authority over their own decisions.

## Structural, accessibility and visual verification

- One Letter portrait section in each file; audited margins and linked-state checks passed.
- Footer contains exactly one `PAGE` and one `NUMPAGES` field.
- Word Heading 1/2 styles form the complete semantic hierarchy.
- Every table repeats its header row; individual rows do not split across pages.
- No inline or floating images are present.
- Accessibility audit: **0 high, 0 medium, 0 low** findings in both files.
- All 53 rendered pages were visually inspected, including table continuations and final change-log pages.
- No clipping, overlap, missing rows, broken headers, unreadable footer or unintended empty page was found.
- Final counters render as `27 | 27` for RU and `26 | 26` for DE.

The bundled LibreOffice renderer was unavailable because `soffice.exe` is not installed. Microsoft Word hidden export and Poppler page rendering were used for field-aware visual QA.

## Integrity

| File | SHA-256 |
|---|---|
| RU DOCX | `2598225BB5B5706B9759E9BC1EBD2CC679E46F56F2B4A74DA796DC7F63F2446C` |
| DE DOCX | `90A49E828DBDF0CFA804D5942A39782DC20C0C2C9F246FBC26CEFF9AB50ECCC5` |

## Publication decision

STD-001 v1.0 is accepted as the current normative World Engine glossary for Foundation Gate F1. It governs new public documentation and machine-facing terminology while preserving compatibility of already published public identifiers until controlled migration.
