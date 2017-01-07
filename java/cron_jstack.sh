#! /bin/bash

mkdir -p jstack
while true
do
    HILUX_ID=`ps axu | grep java | grep hilux | awk -F ' ' '{print $2}'`
    DATE=`date '+%H:%M:%S'`
    JSTACK_STR=`/opt/java/bin/jstack $HILUX_ID > jstack/${DATE}.js`
    sleep 1s
done
