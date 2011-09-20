#!/bin/bash

DIR="$( cd -P "$( dirname "$0" )" && pwd )"

WORKING_DIR= pwd

killall oceanvisserver &> /dev/null
exec ./src/server/oceanvisserver "${DIR}/../../examples/test.conf" &> /dev/null &

# echo "Now starting to ask the server!"

mkdir ~/.local/share/oceanvisserver-test &> /dev/null

until `wget -O ~/.local/share/oceanvisserver-test/coverages.xml "http://127.0.0.1:8080/ovp?SERVICE=ovp&VERSION=1.0.0&REQUEST=DescribeCoverages" &> /dev/null`
do
#     echo "Retrying in 1 second..."
    sleep 1
done

# echo "Now the server is able to answer."

function testServer {
    for COUNT in {1..10}
    do
        BEFORE=`date +%s%N`

        END=250

        for X in $(seq $END)
        do
            let "DAY = $X%${COUNT} + 1"
            wget -O ~/.local/share/oceanvisserver-test/${X}.png "http://127.0.0.1:8080/ovp?SERVICE=ovp&VERSION=1.0.0&REQUEST=GetCoverage&COVERAGEID=$1&SUBSET=time(2011-01-${DAY}T00:00:00+00:00)&SUBSET=height(0)" &> /dev/null
        done

        AFTER=`date +%s%N`

        let "AFTER -= BEFORE"
        let "AFTER /= 1000000"
        echo ${AFTER}
    done
}

echo "Small:"
testServer small
echo "SmallMedium:"
testServer smallmedium
echo "Medium:"
testServer medium
echo "Large:"
testServer large


killall oceanvisserver &> /dev/null
exit 0

