# BetweennessCentrality

## Building

To build the project with both betweeness centrality binaries, run

```
make
```

To build just one, use

```
make EXE
```

where EXE is replaced with either `edge_bt` or `vertex_bt`.

## Running

Source `setup.sh`. Run the binary with the appropriate `OMP_NUM_THREADS`
environment variable or execute `test.sh` if you have `slurm`.
