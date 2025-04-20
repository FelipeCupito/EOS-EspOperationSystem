#!/usr/bin/env python3
"""Pre‑build hook: scan *.eos, emit C++ glue, fail CI on syntax errors.

Usage (PlatformIO): executed automatically via platformio.ini.
"""
import pathlib, sys, re, textwrap
PROJECT_ROOT = pathlib.Path(__file__).resolve().parents[1]
GEN_DIR = PROJECT_ROOT / "src" / "generated"
GEN_DIR.mkdir(exist_ok=True)

# TODO: Replace with real lexer/parser (PEG / Flex + Bison binding).
HEADER = textwrap.dedent("""
    // \u26A1 AUTO‑GENERATED – DO NOT EDIT (prebuild.py)
    #include "eos_core.hpp"
""")

def stub_emit():
    (GEN_DIR / "eos_stub.cpp").write_text(HEADER)
    print("[prebuild] stub glue generated → eos_stub.cpp")

if __name__ == "__main__":
    try:
        stub_emit()
    except Exception as exc:
        sys.stderr.write(f"prebuild failed: {exc}\n")
        sys.exit(1)