# AUDIT-011 — ARCH-010 Source Integrity Correction

**Project:** World Engine  
**Audit branch:** `audit/architecture-consistency-2026-09-01`  
**Status:** Critical source-integrity finding confirmed  
**Date:** 2026-09-01

## 1. Correction to earlier audit assumption

Earlier audit notes stated that the malformed ARCH-010 Complete document could be repaired by replacing the duplicated Part 01 with an existing real Part 02 source.

A direct content comparison of the available source files shows that this assumption was incorrect.

The files named:

- `ARCH-010_Plugin_Architecture_Part_02_Discovery_Detection_RU_v1.docx`
- `ARCH-010_Plugin_Architecture_Part_02_Discovery_Detection_DE_v1.docx`

contain the **same substantive body as Part 01**. The only meaningful difference is the footer/metadata field identifying the file as Part 02.

Therefore the repository/current source set does **not** contain a genuine Part 02 body that can simply be re-merged.

## 2. Confirmed structural state

For both RU and DE source sets:

- Part 01 — genuine Introduction/Scope content;
- Part 02 — duplicated Part 01 body with Part-02 footer metadata;
- Parts 03–08 — distinct content matching their declared subjects;
- Complete document — consequently contains duplicated Part-01 content where Part 02 should be.

This upgrades the previous ARCH-010 repair problem from a simple merge defect to a **missing-source-content defect**.

## 3. Audit impact

The following previous statements are superseded:

- “the actual Part 02 source exists separately”;
- “ARCH-010 can be repaired by inserting the existing Part 02 source”.

The corrected statement is:

> No authentic Part 02 body is currently available in the audited source set. ARCH-010 Part 02 must be reconstructed as a new audited replacement from the established Plugin Architecture requirements and architecture baseline, and must be explicitly marked as reconstructed rather than recovered.

## 4. Reconstruction constraints

Any reconstructed Part 02 must be grounded in:

- ADR-006 Plugin Detection and its clarification;
- ARCH-010 Parts 01 and 03–08;
- ARCH-004/005 dependency and ownership rules;
- ARCH-007 audited plugin-detection sequence;
- ARCH-009 typed configuration boundary;
- ARCH-011 Save compatibility ownership where plugin fingerprint/profile evidence is consumed.

It must not invent executable plugin semantics or make Plugin Detection a Core/Scheduler-owned subsystem.

## 5. Required Part 02 scope

The reconstructed Part 02 should cover only the missing architectural concern implied by its declared title:

**Plugin Discovery, Detection & Change Tracking**

At minimum it must define:

- allowed discovery sources and source trust classification;
- discovery cycle ownership by Application Host / Composition Root;
- separation of Discovery vs Detection vs Metadata Extraction;
- stable plugin identity and normalized artifact identity;
- fingerprint/change-detection strategy at architecture level;
- added/removed/changed/unchanged classification;
- duplicate/path/case handling principles;
- bounded scanning and resource limits;
- initial scan vs subsequent scan behavior;
- failure handling and partial evidence rules;
- detection result contract passed to later metadata/validation stages;
- no arbitrary code execution;
- no compatibility-policy ownership;
- event/publication boundary only after validated evidence is available.

Concrete Skyrim file formats, hashing algorithm choice, filesystem watcher implementation and scan interval remain later implementation decisions unless already fixed by another accepted ADR.

## 6. Repository policy

Do not replace the current ARCH-010 Complete files on `main` until:

1. reconstructed Part 02 RU+DE are created;
2. RU/DE semantic parity is checked;
3. Parts 01–08 are merged in correct order;
4. the rebuilt Complete documents are rendered and visually QA-checked;
5. Plugin lifecycle/ownership corrections from the architecture audit are incorporated;
6. the result passes cross-check against ADR-006 and ARCH-004/005.

## 7. Severity

**Critical** — canonical Complete documents are structurally incomplete and the originally assumed repair source is itself invalid.
