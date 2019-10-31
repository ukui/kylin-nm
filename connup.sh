#!/bin/bash
export LANG='en_US.UTF-8';export LANGUAGE='en_US';nmcli connection up "$1" &> /tmp/kylin-nm-btoutput_
