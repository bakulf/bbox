#!/bin/bash

max=30
folder=stressTest
[ "$2" = "" ] || max=$2
[ "$3" = "" ] || folder=$3

withTimeout=1
[ "$3" = "no" ] && withTimeout=0

function die {
    echo "$1"
    cd ..
    [ -d $folder ] && rm -rf $folder
    exit 1
}

function sequence {
    for ((index=1; $index < $1; index++)); do
        echo $index
    done
}

function mySleep {
    sleep $1 &>/dev/null
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

        mySleep .10

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
rm -rf $folder || die
mkdir $folder || die
cd $folder || die
echo "done."

echo "Let's start with the stress test!"

for l in `sequence 5`; do
    ((timeout=$l - 1))

    echo
    echo Timeout 0.$timeout
    echo -n ". Random files (loop $l)... "
    for i in `sequence $max`; do
        touch $l"_file_"$i
        mySleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Random empty directories (loop $l)... "
    for i in `sequence $max`; do
        mkdir $l"_dir_"$i
        mySleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Moving files in directories (loop $l)... "
    for i in `sequence $max`; do
        mv $l"_file_"$i $l"_dir_"$i
        mySleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    echo -n ". Moving dirs in directories (loop $l)... "
    for i in `sequence $max`; do
        ((next=i+1))
        [ -d $l"_dir_"$next ] && mv $l"_dir_"$i $l"_dir_"$next
        mySleep 0.$timeout
    done
    waitSvnStatus || die "Failed!"
    echo "ok."

    cd $l"_dir_"$i || die "Failed!"
done
