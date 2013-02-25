#!/bin/sh

set -e

mkdir -p \
    roomdata/saved \
    corpses \
    mobdata/repairs \
    immortals

mkdir_atoz () {
    mkdir $1
    # would be simpler with bashism {a..z} but oh well
    for c in a b c d e f g h i j k l m n o p q r s t u v w x y z ; do
        mkdir $1/$c
    done
}

for d in rent account player ; do
    mkdir_atoz $d
done
