# BetweennessCentrality

JMU CS470 project by Brittany Braswell, Jake Hawkins, and Kyle Laker. Based
on "Analysis of parallel implementations of centrality algorithms" by
Patricia D. Soriano, Kevin H. Amrein, Sam P. Carswell, and Michael O. Lam.

[Code for previous work](https://github.com/kevamrein/CS470Project)

## Building

To build the project with both betweeness centrality binaries, run

```
make
```

To build just one, use

```
make EXE
```

where EXE is replaced with either `edge_bt` or `vertex_bt` or anything else in
the Makefile

## Running

Source `setup.sh`. Run the binary with the appropriate `OMP_NUM_THREADS`
environment variable or execute `test.sh` if you have `slurm`.

All executables take the name of a single dataset file in `bc/data`. Either
an absolute path or relative path from CWD must be supplied.
