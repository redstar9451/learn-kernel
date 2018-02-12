# Comment linux kernel code
learn linux kernel with qemu simulator

## how to use
```
./download.sh
make busybox
make vexpress_kernel
./qemu-kernel.sh
```
## qemu options you can speicify:
* -S hold in the first instruction

## how to debug with gdb
waiting for composing

## about making rootfs
* how many ways to identify a rootfs ?

## how to use tap netdevice in qemu

-net tap,vlan=0,name=tap0

```
sudo apt install uml-utilities bridge-utils
sudo brctl addbr br0
sudo brctl stp br0 on
sudo brctl addif br0 ens33
sudo ifconfig ens33 10.1.1.1
sudo dhclient br0

sudo tunctl -t tap0 -u redstar

sudo ifconfig tap0 promisc up
sudo ifconfig tap0 10.1.1.2


sudo brctl delif br0 ens33
sudo brctl delif br0 tap0
sudo tunctl -d tap0
sudo ifconfig br0 down
sudo brctl delbr br0

```

## Questions
* 什么情况下会使用到skb链表？处理分片时？
* skb-\>truesize 有啥用
* _____alloc_skb 中，skbuff_head_cache skbuff_fclone_cache 的区别，为fast clone专门设计了一个memory ?
* skb 是什么的缩写 ? socket buffer
* what meaning does skb->data_len represent ?
