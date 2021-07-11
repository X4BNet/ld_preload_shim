# ld_preload_shim
A LD_PRELOAD shim for forcing a Linux application to bind to an address as detailed at https://www.x4b.net/kb/BindProcessToIPonLinux

Contains:
 - bind.c: Set the environmental variable `BIND_ADDR` to force the socket to be bound to `BIND_ADDR`
 - lowlatency.c: All sockets will use `TCP_QUICKACK` and `TCP_NODELAY`