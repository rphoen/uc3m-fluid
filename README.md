# UC3M Computer Arch 23/24 Sem 1: Fluid Simulation

### To build

```
mkdir cmake-build-debug && cd cmake-build-debug && cmake ..
cmake --build cmake-build-debug
```

run

```
cmake-build-debug/fluid/fluid 1 small.fld out/test.fld
```

### Clang-tidy fixes

np -> npnp
px, py, pz -> posx, posy, posz
ax, ay, az -> accx, accy, accz
hv -> vectorhv
hvx -> vectorhvx,
