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

<!--
## Introduction

- 1 page
- Problem overview and motivation. What are you planning to achieve, what kind of impact do you expect? 
- Review some related work in the literature. (You can borrow parts from your abstract).

-->

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

<!--
- 1-1.5 pages
- Details about the problem and encountered/expected challenges that need solutions.
- Approach(es) you have taken so far - problem decomposition, load balancing, algorithms used, etc. 
- What could be other solution approaches, if any?
-->

## Input parsing

The most popular data format for sequences is FASTA,
a human readable text-based representation.
It is very easy to parse,
but also very inneficient since it doesn't allow any direct form of indexing or random access.
Usually files are processed into more efficient formats before being used,
although it might not be a viable option:
sequence datasets tend to be very large,
and many systems don't have enough disk space to hold both original data and the processed file.

Since the sequential HyperLogLog implementation is CPU-bound (limited by hashing),
optimized input reading is not a priority initially.
Because order is not important to HyperLogLog,
one way to explore parallelism is by using chunked reading (and the pread() system call)
to parse the input file in parallel.

## HyperLogLog update operation

The update operation involves the calculation of a hash function of the input,
some bitwise operations to determine special properties (longest run on 1 bits, for example)
and a memory update to one position of the bit array.
The only shared resource is the bit array,
but to avoid any synchronization instead of having one HyperLogLog shared between threads
(and so a critical section or atomic update of the bit array position)
I opted for creating one HyperLogLog data structure for each thread.
This way the resource is not shared,
but when the updates are over a merge operation must be performed for the final result.

## HyperLogLog merge operation

The merge operation is a elementwise max-reduction between two bit arrays.
Because the bit arrays are relatively small (about 128 KB) the merge operation is an excellent way to avoid resource sharing and synchronization during the update operations,
at the cost of instantiating additional temporary HyperLogLog structures.
Since their sizes are small,
it is a viable tradeoff.

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


## Initial results

<!--
- 1 page
- Describe the programming languages, tools or libraries used.
- Describe the target architecture (multicore CPUs, Xeon Phis or GPUs?). Provide details even if you are using the systems at HPCC, think as if you are writing this for a general audience.
- Provide initial performance results - serial performance vs. parallel performance, strong scaling study, etc. How do you define/evaluate the success for your parallelization efforts?
-->

I implemented parallelization in shared memory using OpenMP.
Shared memory parallelization is useful because the most time consuming
step is calculating the hash and this doesn't share state with other calculations,
and so the critical operation is updating the bit arrays,
which is fast and just modify a small amount of memory.

The target architecture is multicore CPUs.
The primary users of khmer are biologists and one of the aims of the software is easy installation and low barrier to start using.
Compiling in a consistent way for Xeon Phi or GPUs is non-trivial in most systems,
and even OpenMP is not supported in some popular platforms (OSX + clang, for example).
Nonetheless,
adapting the code to use either Xeon Phi or GPUs could lead to even better results,
since the hashing process is mostly CPU-bound.





I intend to use the [Iowa corn soil metagenome dataset][3] as benchmark.
This dataset consist of about a billion reads,
totaling 300 GB of data.
During the implementation I will use a subset,
just so it is fast to run tests.

# How would you evaluate the success of your project?

The SC14 article I cited previously contains some benchmarks,
although with different datasets.
I can't go so far as they did,
since they had access to a supercomputer (NERSC's Edison) with more
cores than we have available at MSU HPCC.

Another important measure is how long it takes to run compared
to Bloom filters construction,
since the whole purpose of doing cardinality estimation is to
optimize the memory consumption of Bloom filters.
It should take only a small fraction of the time needed to build
the proper Bloom filter,
or else it is too expensive to be useful.

## Future Work

<!-- 

- 0.5 pages
- Future work planned until the final report. Division of labor among team members. 

-->

The second part is extending the shared memory implementation with MPI.
The challenge in this part is doing efficient input communication
or even parallel I/O,
since usually the inputs are FASTA files,
which is a text format that need to be parsed sequentially.

[1]: http://www.eecs.berkeley.edu/~egeor/sc14_genome.pdf
[2]: https://github.com/ged-lab/khmer/pull/257
[3]: http://metagenomics.anl.gov/metagenomics.cgi?page=MetagenomeProject&project=6368
