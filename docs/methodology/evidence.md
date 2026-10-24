# Evidence policy

Every technical claim in this repository carries one of four tags. The tag says
how the claim is known, not how important it is. A strong sentence with a weak
tag stays weak.

## Tags

| Tag | Meaning |
|---|---|
| VALIDATED | Reproduced by a recorded run, read directly from the image or source, or confirmed by two independent observations. |
| INFERRED | Follows from validated facts, but the specific conclusion was not directly measured. |
| UNVALIDATED | Plausible and testable, but not tested. Do not treat it as a result. |
| BLOCKED | Needs access or a condition that is not available, such as an unlocked unit, a second device, or a destructive test. |

Tags apply per claim. A document can mix all four. A tag does not spread to
neighboring sentences and does not improve with repetition.

## Path citations

A claim cites a repository-relative path and a line, section, or named log line
when the evidence is public. Examples:

- `docs/exploit/p0-derivation.md`, section "Worked examples"
- `evidence/root-success.md`, credential patch lines
- `docs/lab/qemu.md`, forced-miss results

Private raw captures are retained in the audit store and are not published. A
public document may summarize a private capture and cite the public summary in
`evidence/`. It does not link to a private path and does not quote raw data that
contains identifiers.

## Sanitized evidence

The public evidence files are selected excerpts, not full logs. Sanitization
removes or redacts:

- device identifiers and radio identifiers
- network addresses, SSIDs, and account identifiers
- ADB authorization material and keys
- host paths and usernames
- boot identifiers and process identifiers unless the claim needs them
- long kernel addresses that are not required for the claim
- module lists, vendor blobs, firmware, dexdumps, and private source

The removed material stays in the private audit store. Nothing in the public
tree is a redacted copy of a whole capture.

Some signatures are preserved because the claim depends on them. The
`module_put` and `__fput` frames in `evidence/incidents.md` are an example. The
long fault address is redacted; the function and offset are kept.

## Historical claims

A run is historical when a later boot or incident invalidated its runtime
state. Historical claims keep their original tag and are labeled with the date
and boot context. A later result does not retroactively make an earlier run
stable, and an earlier success does not prove the current state.

The public status is "root demonstrated per boot, not a persistent root." A
claim that the device is rooted right now needs a current session record. The
default is not to claim it.

## Errata

When a claim is wrong, the correction is written next to the old claim or in a
new section. The old text is not silently deleted. This repository has several
superseded claims:

- fixed P0 values were wrong because the loader randomizes the load
- the `pre_fops=0` miss was not only a timing problem
- a process-wide watchdog bounded a stall but was unsafe
- root from an earlier boot does not survive a reboot

The errata rule exists because the audit record is more useful when the dead
ends are visible.

## What is not evidence

- A clean build is not a working exploit.
- A QEMU root is not a phone root.
- A smoke test is not a stability result.
- A short watch window after a fix is not proof that a race is closed.
- A repeated assertion is not a second observation.

## Repository boundary

The public tree contains the analysis, the sanitized excerpts, the build inputs,
and the hashes. The private tree contains the raw captures, firmware, vendor
source, keys, and the full exploit tree. The boundary is deliberate. See
[safety.md](safety.md) for the limits on running any of it.
