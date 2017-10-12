#!/usr/bin/env bash
make
avrdude -p m328p -c usbasp -b 115200 -U flash:w:bin/blink.hex