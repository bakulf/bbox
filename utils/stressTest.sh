#!/bin/bash

max=30
[ "$2" = "" ] || max=$2

withTimeout=1
[ "$3" = "no" ] && withTimeout=0

function die {
    echo "$1"
    cd ..
    [ -d stressTest ] && rm -rf stressTest
    exit 1
}

function sequence {
    seq 1 $1 2>/dev/null || jot $1
}

function waitSvnStatus {
    status=0
    loop=0
    echo -n '.';
    while :; do
        case $status in
            0|4) echo -en '\bo'; ((status++)) ;;
            1|3) echo -en '\bO'; ((status++)) ;;
            2)   echo -en '\b0'; ((status++)) ;;
            5)   echo -en '\b.'; status=0 ;;
        esac

        sleep .10

        if [ "$(svn status 2>&1)" = "" ]; then
            echo -en '\b'
            return 0
        fi

        ((loop++))
        if [ $withTimeout = 1 ] && [ "$loop" -gt 200 ]; then
            echo -en '\b'
            return 1
        fi
    done
}

echo ".: StressTest for BBox :."
echo "Remember: BBox must be running and in autocommit."
echo

if [ "$1" = "" ] || ! [ -d "$1" ]; then
    echo "Usage: $0 <dirUnderSvn>"
    exit 1
fi

cd "$1" || die

echo -n "Creating the tmp directory... "
rm -rf stressTest || die
mkdir stressTest || die
cd stressTest || die
echo "done."

echo "Let's start with the stress test!"

for l in `sequence 5`; do
    ((timeout=$l - 1))

    echo
    echo Timeout 0.$timeout
    echo -n ". Random files (loop $l)... "
    for i in `sequence $max`; do
        touch $l"_file_"$i
        sleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Random empty directories (loop $l)... "
    for i in `sequence $max`; do
        mkdir $l"_dir_"$i
        sleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Moving files in directories (loop $l)... "
    for i in `sequence $max`; do
        mv $l"_file_"$i $l"_dir_"$i
        sleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Moving dirs in directories (loop $l)... "
    for i in `sequence $max`; do
        ((next=i+1))
        [ -d $l"_dir_"$next ] && mv $l"_dir_"$i $l"_dir_"$next
        sleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    cd $l"_dir_"$i || die "Failed!"
done
