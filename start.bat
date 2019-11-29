@echo on

call stop.bat

set GOPROXY=https://goproxy.io
set GOBIN=%~dp0bin

go install ./GoCallDLL

