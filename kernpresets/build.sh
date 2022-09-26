#!/bin/bash

function Seq {
    min=$1
    max=$2
    while [ $min -le $max ]; do
        echo $min$3
        min=$((min+1))
    done
}

function Square {
    Seq 1 4 $1
}

function Diamond {
    Seq 1 4 $1
}

function Octagon {
    Seq 1 5 $1
}

function Disk {
    for i in 0.5 1.0 1.5 2.0 2.5 2.9 3.5 3.9 4.3 4.5 5.3; do echo $i$1; done
}

function Plus {
    Seq 1 4 $1
}

function Cross {
    Seq 1 4 $1
}

function Ring {
    for i in 1 1.5 1,1.5 2 1,2 1.5,2 1,2.5 1.5,2.5 1.5,2.9 2,2.5 2,3.5 2.5,3.5 2.9,3.5 3,3.5 3,3.9 2.5,4.3 3.5,4.3 3.9,4.5 4,4.5 4.3,4.5 4.3,5.3; do echo $i$1; done
}

function Rectangle {
    for i in 2x2 3x3 4x2 4 7x3 7x1 7x1+1+0 7x1+6+0; do echo $i$1; done
}

convert xc: -bordercolor black -border 6x6 pixel.gif

for k in Square Diamond Octagon Disk Plus Cross Ring Rectangle; do
    echo -n "$k: ["
    echo -n `$k ", "`
    echo "],"
    for r in `$k`; do
        convert pixel.gif -morphology Dilate $k:$r -scale 800% -negate -write mpr:kern +delete \
            -size 8x8 xc:white -draw 'rectangle 1,1 7,7' -write mpr:block +delete \
            mpr:kern -size 105x105 tile:mpr:block \
            +swap -compose screen -composite -negate \
            -fill red -draw 'circle 52,52 53,53' -colors 4 kern_$k-$r.gif
    done
done

rm pixel.gif