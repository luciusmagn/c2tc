#!/bin/sh
groff -W number -k -Tutf8 -dpaper=a3 -man c2tc_cs.1 | less -r
