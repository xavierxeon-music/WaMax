# Decoding Max/MSP "Compressed Patcher" Clipboard Text

Max (Cycling '74) can export a patcher — or any selection of objects — as a block
of compressed text for pasting into forums, emails, or chat. It looks like this:

```
----------begin_max5_patcher----------
477.3ocsUEkaBCCC8eNEQ46NDEJLXWEDBEZMPlZSpRbq.g3tuDSAJaAnHzpP
VoNO6D7yO2C8XtG9J8Nvx+hMmd0+b3xpy.baeqSZiBwtzbg0GLu.rVwFfG8W
...
-----------end_max5_patcher-----------
```

In Max itself you decode it by selecting the whole block (including the
begin/end marker lines) and choosing **File > New From Clipboard**. This
document describes the underlying byte format, so it can be decoded
programmatically without Max running.

## Format overview

1. Strip the `----------begin_max5_patcher----------` and
   `-----------end_max5_patcher-----------` marker lines.
2. Join the remaining lines into one continuous string (the line breaks are
   just word-wrap and carry no meaning).
3. The string starts with a decimal number followed by a period, e.g. `477.` —
   this is the **byte length of the zlib-compressed patcher JSON**, before
   base64-style encoding. Everything after that first `.` is the encoded
   payload.
4. The payload is a **non-standard base64 variant** of zlib-compressed JSON
   (the patcher's `.maxpat` JSON representation).
5. Decoding the payload and running it through zlib inflate yields the raw
   patcher JSON (`boxes`, `lines`, `appversion`, etc. — the same structure as
   a `.maxpat` file).

Cycling '74 staff have confirmed on the forums that this is "a non-standard
variation of base64 encoded gzip'ed JSON" — the specifics of that variation
(alphabet + bit order) had to be reverse-engineered, since it isn't publicly
documented. Details below.

## The base64 variant

It differs from standard base64 (`A-Za-z0-9+/`) in two ways:

- **Alphabet**: the 64th symbol is `.` instead of `/`. Full alphabet, in
  index order:

  ```
  .ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+
  ```

  Note the ordering: index 0 is `.`, then the usual `A-Za-z0-9`, then `+` at
  index 63 (i.e. the alphabet is the standard one rotated by one position,
  with `/` swapped for `.`).

- **Bit packing is least-significant-bit first**, not the standard
  most-significant-bit-first packing used by RFC 4648 base64. Each character
  contributes 6 bits starting from the low end of the output byte stream,
  rather than the high end.

- No `=` padding is used; the leading byte-length prefix (`477.`) tells you
  exactly how many bytes to expect, so the decoder just stops when it's
  produced that many bytes.

## Reference implementation (Python)

```python
import zlib

_ALPHABET = ".ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+"
_INDEX = {c: i for i, c in enumerate(_ALPHABET)}


def _decode_lsb_base64(s: str) -> bytes:
    """Decode Max's non-standard, LSB-first base64 variant."""
    out = bytearray()
    buf = 0
    nbits = 0
    for c in s:
        buf |= _INDEX[c] << nbits
        nbits += 6
        while nbits >= 8:
            out.append(buf & 0xFF)
            buf >>= 8
            nbits -= 8
    return bytes(out)


def decode_max_clipboard(text: str) -> dict:
    """Decode a Max 'compressed patcher' clipboard block into patcher JSON."""
    # Strip marker lines and whitespace, join into one string.
    lines = [
        line.strip()
        for line in text.splitlines()
        if line.strip() and "max5_patcher" not in line
    ]
    data = "".join(lines)

    # First '.' separates the declared byte length from the payload.
    dot_pos = data.index(".")
    declared_len = int(data[:dot_pos])
    payload = data[dot_pos + 1:]

    compressed = _decode_lsb_base64(payload)
    assert len(compressed) == declared_len, "length mismatch — bad input?"

    json_bytes = zlib.decompress(compressed)
    import json
    return json.loads(json_bytes)


if __name__ == "__main__":
    import sys
    with open(sys.argv[1]) as f:
        raw = f.read()
    patch = decode_max_clipboard(raw)
    import json
    print(json.dumps(patch, indent=2))
```

Usage: save the clipboard block (marker lines included or not — both are
handled) to a text file and run `python decode_max_clipboard.py patch.txt`.

## Encoding (reverse direction)

To go the other way — JSON → compressed clipboard text — reverse each step:

1. `json.dumps(patch)` → UTF-8 bytes.
2. `zlib.compress(...)` → compressed bytes. Record `len(compressed)`.
3. Encode with the same alphabet, but pack bits **LSB-first** in groups of 6,
   emitting one character per 6 bits (pad the final partial group with zero
   bits, no `=` padding character).
4. Prefix with `f"{len(compressed)}."`.
5. Wrap at ~60 characters per line and add the begin/end marker lines if you
   want it to look like a real Max clipboard export.

## Notes / gotchas

- The declared length is the **compressed** byte count, not the original
  JSON size — useful as a sanity check after decoding (should match exactly).
- This format is sometimes called the "Max 5 patcher" format
  (`begin_max5_patcher`/`end_max5_patcher` markers) even in patches saved
  from much newer versions of Max — the marker name is legacy, not a version
  indicator. The actual Max version used is inside the decoded JSON under
  `appversion`.
- This is different from the `.amxd` binary format used for Max for Live
  device files, and different from the plain-text `max v2;` / `#N vpatcher`
  format used by very old (Max 4-era) patches — those are uncompressed and
  need no decoding at all, just File > Open as Text.
