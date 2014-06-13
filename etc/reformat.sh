#!/bin/sh

astyle \
    --style=kr \
    --indent=spaces=4 \
    --indent-preprocessor \
    --min-conditional-indent=0 \
    --pad-oper \
    --pad-header \
    --break-blocks=all \
    --unpad-paren \
    --keep-one-line-blocks \
    --keep-one-line-statements \
    --align-pointer=name \
    --align-reference=name \
    --lineend=linux \
    --suffix=none \
    --ignore-exclude-errors-x \
    $*
