# DOC-009 v1.0 — Full-document QA

**Date:** 2026-09-05

**Branch:** `docs/pre-code-foundation-plan`

**Result:** PASS

## Published artifacts

- `DOC-009_Developer_Journal_RU_v1_0_Current.docx`
- `DOC-009_Developer_Journal_DE_v1_0_Current.docx`
- Matching editable RU and DE Markdown sources are retained next to the Word pair.

## Content verification

| Check | RU | DE | Result |
|---|---:|---:|---|
| Rendered pages | 21 | 21 | PASS |
| Tables | 15 | 15 | PASS |
| Table rows | 119 | 119 | PASS |
| Semantic headings | 46 | 46 | PASS |
| Footer `PAGE` fields | 1 | 1 | PASS |
| Footer `NUMPAGES` fields | 1 | 1 | PASS |
| Initial evidence records | Present | Present | PASS |
| MOD-001–MOD-014 register | Complete | Complete | PASS |

The pair establishes an append-only, evidence-based living journal with entry types, lifecycle, identifiers, correction rules, Git linkage, risk/experiment/blocker/release/handoff templates and verified initial project history. The module register maintains independent document, implementation and validation states for MOD-001–MOD-014.

## Structural and accessibility verification

- One Letter portrait section in each file.
- The footer contains exactly one `PAGE` and one `NUMPAGES` field.
- Word heading styles express the complete semantic hierarchy.
- Every table repeats its first-row header and prevents individual rows from splitting across pages.
- RU and DE structural metrics are identical.
- Accessibility audit: **0 high, 0 medium, 0 low** findings in both files.
- Page 21 renders as `21 | 21` in both languages.

## Visual verification

Every page of both documents was rendered and inspected. The review covered the cover, document control, contents, the 30 journal-policy sections, seven initial evidence records and appendices A–E. The initial generic cover label was corrected to `Current / Living`; the pair was rebuilt, exported and inspected again. The final files contain no clipping, overlap, orphaned section heading, broken table, unexpected blank page or RU/DE layout divergence.

The bundled LibreOffice renderer was unavailable because `soffice.exe` is not installed on this host. Microsoft Word hidden export was used for field-aware PDF generation, followed by Poppler rendering of all pages to PNG.

## Integrity

| File | Bytes | SHA-256 |
|---|---:|---|
| RU DOCX | 2,082,489 | `E347761DC954EC5679F2D03C2B78D53F77858CD092CBF56298185845D9575007` |
| DE DOCX | 2,081,102 | `29A7677EA92547E884AD7E7B98D75E23FE7CC13C60181FEF07423501B68D0222` |

## Publication decision

DOC-009 v1.0 is accepted as the current living developer journal. Its publication closes the project-management register at `9 / 9`. Future material changes must add evidence-linked journal entries; corrections preserve the earlier record instead of silently rewriting history.
