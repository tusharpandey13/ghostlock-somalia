#!/bin/sh
# Fail on common publication hazards. This is a guard, not a complete scanner.
set -eu
fail=0
scan() {
  name=$1; pat=$2
  if rg -n --hidden -g '!.git/**' -g '!LICENSE' -g '!.gitignore' -g '!scripts/check_public.sh' -e "$pat" . >/tmp/gl_scan.$$ 2>/dev/null; then
    echo "FAIL: $name"
    head -20 /tmp/gl_scan.$$
    fail=1
  fi
}
# Device identifiers / owner data (generic patterns; do not put real values here).
scan "15-digit IMEI-like" '\b[0-9]{15}\b'
scan "device serial-like" '\bC[0-9A-F]{10,12}\b'
scan "MAC address" '\b([0-9A-Fa-f]{2}:){5}[0-9A-Fa-f]{2}\b'
scan "RFC1918 IP" '\b(10|192\.168|172\.(1[6-9]|2[0-9]|3[01]))\.[0-9]{1,3}\.[0-9]{1,3}\.[0-9]{1,3}\b'
scan "private key" 'BEGIN (RSA |OPENSSH |EC |DSA )?PRIVATE KEY'
scan "ADB key filename" 'adbkey(\.pub)?'
# .gitignore intentionally names the pattern; exclude it from the key scan.

scan "host home path" '/Users/[A-Za-z0-9._-]+/'
rm -f /tmp/gl_scan.$$
exit "$fail"
