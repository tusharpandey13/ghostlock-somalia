# Twitter / X

Draft, replace the URL before posting. No serial, no IP, no identifiers. Link
counts as 23 characters on X.

```text
Rooted my Redmi A7 Pro 5G (T8300, Android 16, 6.6.118) with GhostLock CVE-2026-43499: shell -> uid 0, per-boot, no boot image change. The non-obvious part was deriving the per-boot physical load from the tracefs KASLR leak. Fix under observation. https://example.invalid/writeup
```

Notes:

- Owned device, owner-authorized, read-only posture.
- Public CVE (CVE-2026-43499), no new disclosure claim.
- Per-boot only, no persistence. The selective-fd fix is under observation.
