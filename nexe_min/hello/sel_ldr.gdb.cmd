file /home/yjy/project/nacl_root/native_client/scons-out/dbg-linux-x86-32/staging/sel_ldr
set args -B /home/yjy/project/nacl_root/native_client/scons-out/nacl_irt-x86-32/staging/irt_core.nexe -g -- /home/yjy/project/workspace/newlib/hello/hello.nexe
break main
break NaClAllocAddrSpaceAslr
break NaClStartThreadInApp 
run 
