#!/usr/bin/env bash
avrdude -p m328p -c usbasp -b 115200 -U flash:w:blink.hex