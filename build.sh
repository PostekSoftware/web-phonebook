#!/bin/bash

mkdir out
mkdir out/htmcpp
mkdir out/httplib
mkdir out/md5
mkdir out/app
clang++ -Wall -shared -fPIC -std=c++20 htmcpp/external/httplib/httplib.cc -Ihtmcpp/external/httplib -Ihtmcpp/external/json -o out/httplib/httplib.so
clang++ -shared -fPIC -std=c++20 htmcpp/external/md5/md5.cc -Ihtmcpp/external/md5 -o out/md5/md5.so 
clang++ -shared -fPIC -std=c++20 app-server/server.cc out/httplib/httplib.so out/md5/md5.so -Iapp-server -Iapp-server/ -Ihtmcpp/external/httplib -Ihtmcpp/external/json -Ihtmcpp/external/inja -Ihtmcpp/external/md5 -o out/app/app.so 
clang++ -std=c++20 htmcpp/build.cc out/httplib/httplib.so out/md5/md5.so -I. -Ihtmcpp/src/ -Iapp-server/ -Ihtmcpp/external/httplib -Ihtmcpp/external/json -Ihtmcpp/external/inja -Ihtmcpp/external/md5 -o out/htmcpp/server