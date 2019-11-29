#!/bin/bash

flag="app"
bash stop.sh


export GOPROXY=https://goproxy.io
export GOBIN=$PWD/bin

go install ./GoCallDLL/src

#cd res/config
#if [ ! -f  "server.toml" ]; then
#	cp server.toml.example server.toml
#fi

#cd ../../

#nohup ./serverapp.sh $flag > ServerApp_`date +%Y-%m-%d_%H-%M-%S`.log &
#ps ux | grep $flag |grep -v grep
