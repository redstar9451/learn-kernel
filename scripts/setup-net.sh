#!/bin/bash
set -x
[ -z $1 ] && echo please sepcify the host netdevice !!!! && exit 0
[ -z $2 ] && echo please sepcify the current username !!!! && exit 0

host_netdev=$1

brctl addbr br0
brctl stp br0 on
# note you will lose the connection because ip of ens33 is invalid
brctl addif br0 $host_netdev && dhclient br0

tunctl -t tap0 -u $2
ifconfig tap0 up

brctl addif br0 tap0
