# manual

## tst_shmem
```bash
Usage: "MAIN_PROG" [option] <dev> [type]
The option are:
       -h, --help      Print help information
       -V, --version   main program version
       --read=n <dev>  read shmem. n is LD index, 0=LD1,1=LD2...
       --set=n <dev>   set shmem. n is LD index, 0=LD1,1=LD2...
       --select n <dev> <type>   select sg, n=1,2,...
The dev is: phy or ld
The type is: act or edt
```

### read
read phydev parameter, then save to **paramphd.txt**
read LDn parameter, then save to **sg4ldn.txt**

### set
read phydev parameter from **paramphd.txt**, then set to shmem
read LDn parameter from **sg4ldn.txt**, then set to shmem

### select
select phydev or ld sgn, not save to file.


