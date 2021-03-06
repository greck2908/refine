#!/usr/bin/env bash

set -x # echo commands
set -e # exit on first error
set -u # Treat unset variables as error

cat > faux_input <<EOF
6
1 xplane 0
2 xplane 1
3 yplane 0
4 yplane 1
5 zplane 0
6 zplane 1
EOF

if [ $# -gt 0 ] ; then
    one=$1/one
    two=$1/src
else
    one=${HOME}/refine/strict/one
    two=${HOME}/refine/strict/src
fi

${two}/ref_acceptance 1 ref_adapt_test.b8.ugrid

function adapt_cycle {
    proj=$1

    cp ref_adapt_test.b8.ugrid ${proj}.b8.ugrid

    ${two}/ref_translate ${proj}.b8.ugrid ${proj}.html
    ${two}/ref_translate ${proj}.b8.ugrid ${proj}.tec

    ${two}/ref_acceptance ${proj}.b8.ugrid ${proj}.metric 0.001

    ${two}/ref_translate ${proj}.b8.ugrid ${proj}.fgrid
    ${one}/refine-px -g ${proj}.fgrid -m ${proj}.metric -o refine-one.fgrid
    ${two}/ref_translate refine-one.fgrid ref_adapt_test.b8.ugrid

    ${two}/ref_metric_test ${proj}.b8.ugrid ${proj}.metric > ${proj}.status
    cp ref_metric_test_s00_n1_p0_ellipse.tec ${proj}_metric_ellipse.tec
}

adapt_cycle accept-3d-one-00
adapt_cycle accept-3d-one-01
adapt_cycle accept-3d-one-02
adapt_cycle accept-3d-one-03

cat accept-3d-one-03.status
../../../check.rb accept-3d-one-03.status 0.03 2.0

