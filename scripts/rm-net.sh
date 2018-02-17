#!/bin/bash
set -x
ifconfig br0 down && brctl delbr br0
tunctl -d tap0

