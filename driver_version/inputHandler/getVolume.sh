#!/usr/bin/env bash
amixer sget 'Master',0 | sed -nE 's/.*\[([0-9]+)%\].*/\1/p' > volume

