# Publishing

This repository is the canonical source for the public work:

- `WRITEUP.md` — the narrative. Blog/domain copies should link back to a tagged
  commit or release here.
- `TIMELINE.md` — the complete sanitized operation journey.
- `STATUS.md` — current claims and limits. Update this before re-posting.
- `docs/`, `reference/`, `evidence/` — the research mine and sanitized evidence.

## Release checklist

1. Update `STATUS.md` and the date if anything material changed.
3. Tag the release, for example `v1.0`.
4. Publish or mirror the writeup; use the repository tag as the canonical
   citation and add a link from the blog page. Social drafts are kept outside
   this repository; only the writeup, timeline, and status are canonical here.
5. Post one platform at a time; follow each community's self-promotion and
   disclosure rules. Do not use a throwaway account to evade them.

## Redaction

Public tree policy is in `STATUS.md` and `docs/methodology/evidence.md`. Run
`./scripts/check_public.sh` before publishing or tagging. Raw captures, device
identifiers, ADB keys, firmware, vendor blobs, and owner data stay private.

## Not claimed

This is a device-specific port of a public CVE, not a new CVE. Root is
per-boot; the selective-fd build is under observation, not a stable product.
