#!/bin/bash

cd "$(dirname "$0")" || exit

set -euxo pipefail

CMAKE_BUILD_RELEASE="../cmake-build-release"

/opt/opencilk/bin/clang++ ../"$1".cpp -fopencilk -fcilktool=cilkscale -O3 -o "${CMAKE_BUILD_RELEASE}"/cs
/opt/opencilk/bin/clang++ ../"$1".cpp -fopencilk -fcilktool=cilkscale-benchmark -O3 -o "${CMAKE_BUILD_RELEASE}"/cs_bench

python3 /opt/opencilk/share/Cilkscale_vis/cilkscale.py \
    -c "${CMAKE_BUILD_RELEASE}"/cs -b "${CMAKE_BUILD_RELEASE}"/cs_bench \
    -ocsv cstable.csv -oplot csplots.pdf \
    --args "${*:2}"
