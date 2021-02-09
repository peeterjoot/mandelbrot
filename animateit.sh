#!/bin/bash

all=`for i in {0..199} ; do echo p/$i.fp.png ; done`

convert -delay 1 \
$all \
-loop 0 animfp3.gif
