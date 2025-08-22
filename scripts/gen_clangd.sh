# SPDX-License-Identifier: BSD-2-Clause
#!/bin/sh

cat > .clangd <<EOF
CompileFlags:
  Add: [-xc, -I$(pwd)/include, -std=c99, -Wall, -Wextra, -Wpedantic]
  RemapCxx: [.cpp, .cc, .cxx, .hxx]
  RemapC: [.c, .h]
EOF
