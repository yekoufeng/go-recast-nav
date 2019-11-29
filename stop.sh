#!/bin/bash


flag="app"

#退出进程
pids=`ps ux | grep $flag | grep -v grep |awk '{print $2}'`
for id in $pids
do
    echo "killPid:" $id
    kill -2 $id
done

echo "Finish kill server..."

ps ux | grep $flag |grep -v grep


