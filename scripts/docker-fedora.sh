#!/usr/bin/env bash
echo $pwd
ls
sudo docker run --rm=true -v `pwd`:rw fedora:${OS_VERSION} "pwd && cd scripts && make"
