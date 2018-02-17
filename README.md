# Comment linux kernel code
learn linux kernel with qemu simulator

## how to use
```
./download.sh
make all
```
## qemu options you can speicify:
* -S hold in the first instruction
* -s gdb server with port 1234

## how to debug with gdb
waiting for composing

## about making rootfs
* how many ways to identify a rootfs ?

## how to use tap netdevice in qemu
first, sudo apt install uml-utilities bridge-utils

### install tap netdevice 
sudo ./setup-net.sh <netdevice> <username>
for example:
sudo ./setup-net.sh eth0 $(whoami)
### remove tap netdevice
sudo ./rm-net.sh

## Tips
* only support assign ip via DHCP
* The IP assignment maybe fail, commonly because DHCP, then you will lose the connection to the host when you login via ssh/telnet

## Questions
* 什么情况下会使用到skb链表？处理分片时？
* skb-\>truesize 有啥用
* _____alloc_skb 中，skbuff_head_cache skbuff_fclone_cache 的区别，为fast clone专门设计了一个memory ?
* skb 是什么的缩写 ? socket buffer
* what meaning does skb->data_len represent ?
