# Crypto solvers

This repo contains solvers for various types of classical cryptograms.
Currently the only solver here is for monoalphabetic substitutions with
no word breaks, but there will be more added soon.

## Building
To build, just do:
```
cd src
make
make install
```

The `make install` command just copies the solvers to the bin directory.

## Running monosolve
To run the monoalphabetic substitution solver, do:
bin/monosolve _your ciphertext file_

It should print out solutions as it computes them, and at some point
will quit if it can't find anything else better than its last solution.
