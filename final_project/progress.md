---
title: Parallel cardinality estimation using HyperLogLog counters
date: November 20th, 2014
author:
 - name: "Luiz Carlos Irber Jr"
   affiliation: "CSE 891-Section 1 (Parallel Computing: Fundamentals and Applications)"
   email: irberlui@msu.edu
bibliography: <!-- \bibliography{bibs/fpbib-pandoc.bib} -->
...

# Introduction

## Overview and motivation

Next-Generation Sequencing (NGS) technologies generate data at increasing rates,
and for almost fifteen years at an even faster rate than Moore's Law.
Algorithms need to be adapted or even developed to deal with these data rates,
and depending on which analysis is intended probabilistic data structures
can be used to manage the complexity.

Probabilistic data structures are useful when the cost for a exact
answer is prohibitive,
but an approximate answer is acceptable.
An example are Bloom filters,
useful for set membership operations.
They basically work by creating a large bit array of $m$ bits and using
$k$ hash functions to update each position of the array.
To insert a new value to a Bloom filter the hash functions are calculated
and the hashed values are used to set positions on the bit array to 1.
Querying this structure means calculating the hash for a value and checking
if all the 1-bit are set in the bit array.
To avoid saturation and returning false positives more memory can be allocated (by increasing $m$),
but ultimately the amount of memory needed to avoid false positives depend
on the cardinality of the set (how many unique elements are present).
Usually this is not known beforehand,
and since there is no way to remove elements or extend a already started
Bloom filter the computation needs to be restarted if the false positive rate gets too high.

One way to avoid this is by estimating the cardinality of the set,
and then initializing a Bloom filter with the proper size.
The HyperLogLog counter is a cardinality estimation data structure with constant (and low) memory footprint,
also based on hashing and probabilistic estimation.
It works by a similar process to Bloom filters,
but with a different way to update a bit array.

Since computers are getting faster by increasing the number of cores
available for computation,
exploring parallelism is essential to be able to process all this data.
HyperLogLog counters can be merged trivially
(by doing a minimum-reduction of the underlying bit array),
and so are very efficient and a perfect fit for parallelization.

## Related work

I have a sequential HyperLogLog counter implementation in a [khmer branch][2].
This can be used as baseline.

There is an [article][1] accepted for Supercomputing 2014 describing the implementation of a parallel HyperLogLog counter.
They use C++ and MPI,
and also MurmurHash as hash function,
the same used for my sequential implementation,

# Methods

## Input parsing

The most popular data format for sequences is FASTA,
a human readable text-based representation.
It is very easy to parse,
but also very inneficient since it doesn't allow any direct form of indexing or random access.
Usually files are processed into more efficient formats before being used,
although it might not be a viable option:
sequence datasets tend to be very large,
and many systems don't have enough disk space to hold both original data and the processed file.

## HyperLogLog update operation

The update operation involves the calculation of a hash function of the input,
some bitwise operations to determine special properties (longest run on 1 bits, for example)
and a memory update to one position of the bit array.
The only shared resource is the bit array.

## HyperLogLog merge operation

The merge operation is a elementwise max-reduction between two bit arrays.
Because the bit arrays are relatively small (about 128 KB) the merge operation is an excellent way to avoid resource sharing and synchronization during the update operations,
at the cost of instantiating additional temporary HyperLogLog structures.
Since their sizes are small,
this is a viable tradeoff.

## Problem decomposition

Since the sequential HyperLogLog implementation is CPU-bound (limited by hashing),
optimized input reading is not a priority initially.
In the future,
Because order is not important to HyperLogLog,
one way to explore parallelism is by using chunked reading (and the pread() system call)
to parse the input file in parallel.
The initial implementation uses one thread to parse the input.

The only shared resource during updates is the bit array.
In order to avoid any synchronization instead of having one HyperLogLog shared between threads
(and so a critical section or atomic update of the bit array position)
I opted for creating one HyperLogLog data structure for each thread.
This way the resource is not shared,
but when the updates are done a merge operation must be performed for the final result.

<!--
TODO:
  use pread(),
  create splits of size filesize/threads
  give each split to one task
    - start reading the split
    - ignore data until a '>' is found
    - process all reads
    - stop processing when another '>' is found AFTER split is over (or EOF)
-->


# Initial results

I implemented parallelization in shared memory using OpenMP.
Shared memory parallelization is useful because the most time consuming
step is calculating the hash and this doesn't share state among other calculations.
The critical operation is updating the bit arrays,
which is fast and just modify a small amount of memory.

khmer is implemented both in Python and C++.
Data structures and performance-critical sections are implemented in C++ and this low level API is exposed to Python,
with scripts providing the functionality available to users.
The Python interpreter is implemented using a global interpreter lock (GIL),
which makes multithreading virtually impossible with pure Python.
Usually multiprocessing is used instead,
but there is a communication overhead (since memory is not shared between processes).
C/C++ extensions are not affected,
and so OpenMP is viable.

The target architecture is multicore CPUs.
The primary users of khmer are biologists and one of the project goals is easy installation and a low cognitive barrier for new users.
Compiling in a consistent way for Xeon Phi or GPUs is non-trivial in most systems,
and even OpenMP is not supported in some popular platforms (OSX + clang, for example).
Nonetheless,
adapting the code to use either Xeon Phi or GPUs could lead to even better results,
since the hashing process is mostly CPU-bound.

The current version of the parallel implementation is available on [GitHub][4].

<!--
Parameters which might affect performance are:
- Read length
- K size
-->

I used two different datasets during development,
one being a subset 3 orders of magnitude smaller than the other:

Gallus_3.longest25.partial.fasta

  - 112,455 basepairs
  - 47 seqs
  - 2392.7 average length
  - 111 KB

Gallus_3.longest25.fasta

  - 149,943,923 bp
  - 44,336 seqs
  - 3,382.0 average length
  - 144 MB

Despite being smaller,
the average length of each sequence is about the same for both datasets.
I used the partial dataset just to make quick tests
(specially when segfaults were envolved).

This is the timing data for the complete dataset (all tests with k=32):

``` bash
  $ export OMP_NUM_THREADS=1
  $ time ./hll ../../Gallus_3.longest25.fasta
  129,388,424
  real    0m52.660s user    0m52.505s sys     0m0.062s

  $ export OMP_NUM_THREADS=2
  $ time ./hll ../../Gallus_3.longest25.fasta
  129,388,424
  real    0m27.193s user    0m54.142s sys     0m0.082s

  $ export OMP_NUM_THREADS=4
  $ time ./hll ../../Gallus_3.longest25.fasta
  129,388,424
  real    0m14.042s user    0m55.790s sys     0m0.093s

  $ export OMP_NUM_THREADS=8
  $ time ./hll ../../Gallus_3.longest25.fasta
  129,388,424
  real    0m7.370s user    0m58.318s sys     0m0.084s

  $ export OMP_NUM_THREADS=16
  $ time ./hll ../../Gallus_3.longest25.fasta
  129,388,424
  real    0m3.773s user    0m59.251s sys     0m0.094s
```

Speedup times are close to linear,
which is best seem on Figure 1.

![Parallel implementation speedup](report_speedup.png "")

For stress testing I used a larger dataset.
Although it's 4 times smaller than the one I proposed to use,
it is a typical dataset found by users.

Chicken_10Kb20Kb_40X_Filtered_Subreads.fastq

  - 43,076,933,303 bp
  - 9,006,923 seqs
  - 4,782.6 average length
  - 81 GB

Using the Python API, with 16 threads and k = 32,
the running time is close to what is expected when extrapolating the
results on smaller datasets,
(about 36 minutes):

``` bash
  $ time python unique_kmers.py Chicken_10Kb20Kb_40X_Filtered_Subreads.fastq 32
  unique k-mers: 41,954,729,591
  real    35m2.600s user    326m27.120s sys     2m53.487s
```

# Future Work

The priority is doing proper benchmarking and find bottlenecks.
I already ran some tests using TAU and the C++ driver program,
but I would like to test in 'real' conditions,
going through the Python API.
It is also essential to run multiple replicates and add error intervals
to all measurements.

Initially I proposed to extend the shared memory implementation with MPI.
Given the additional burden for the project in terms of installation
I'm planning to implement it using MPI4py,
which can be an optional dependency and doesn't affect the compilation process.

Before implementing anything MPI-related,
I still want to explore the parallel input reading (chunked) and check
if it is viable.
I'm expecting worse or at most equal results for a small number of threads,
but this might be better when a large number of threads is used.
An additional approach can use a variable number of readers,
instead of only one (current implementation) or everyone reading (proposal above).

[1]: http://www.eecs.berkeley.edu/~egeor/sc14_genome.pdf
[2]: https://github.com/ged-lab/khmer/pull/257
[3]: http://metagenomics.anl.gov/metagenomics.cgi?page=MetagenomeProject&project=6368
[4]: https://github.com/ged-lab/khmer/blob/3a6dfed60111c807f7c6d26cfc8c5bb52e6f1aca/lib/hllcounter.cc#L320
