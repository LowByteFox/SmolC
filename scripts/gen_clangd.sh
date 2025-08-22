# SPDX-License-Identifier: BSD-2-Clause
#!/bin/sh

NO_WARNS="-Wno-empty-translation-unit"

cat > .clangd <<EOF
CompileFlags:
  Add: [-xc, -I$(pwd)/include, -std=c99, -Wall, -Wextra, -Wpedantic, $NO_WARNS]
  RemapCxx: [.cpp, .cc, .cxx, .hxx]
  RemapC: [.c, .h]
EOF
