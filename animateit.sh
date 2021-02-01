#!/bin/bash

all=`for i in {1..200} ; do echo o/mp2.400.$i.png ; done`

convert -delay 1 \
$all \
-loop 0 animation.gif
