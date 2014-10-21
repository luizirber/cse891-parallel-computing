---
title: Parallel cardinality estimation using HyperLogLog counters
date: October 20th, 2014
author:
 - name: "Luiz Carlos Irber Jr"
   affiliation: "CSE 891-Section 1 (Parallel Computing: Fundamentals and Applications)"
   email: irberlui@msu.edu
bibliography: <!-- \bibliography{bibs/fpbib-pandoc.bib} -->
...

# What is the problem?

Next-Generation Sequencing (NGS) technologies generate data at increasing rates,
and for almost fifteen years at an even faster rate than Moore's Law.
Algorithms need to be adapted or even developed to deal with these data rates,
and depending on which analysis is intended probabilistic data structures
can be used to manage the complexity.

Probabilistic data structures can be used when the cost for a exact
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
exploring parallelism is a must to be able to process all this data.
HyperLogLog counters can be merged trivially
(by doing a minimum-reduction of the underlying bit array),
and so are very efficient and a perfect fit for parallelization.

# What is the state of the art in terms of the existing solutions?

I have a sequential HyperLogLog counter implementation in a [khmer branch][2].
This can be used as baseline.

There is an [article][1] accepted for Supercomputing 2014 describing the implementation of a parallel HyperLogLog counter.
They use C++ and MPI,
and also MurmurHash as hash function,
the same used for my sequential implementation,

# What tools and libraries are you planning to use?

I'll start by implementing a shared memory (OpenMP) parallelization,
since this can be used as a building block for distributed memory.
Shared memory parallelization is useful because the most time consuming
step is calculating the hash and this doesn't share state with other calculations,
and so the critical operation is updating the buckets,
which is fast and just modify a small amount of memory.

The second part is extending the shared memory implementation with MPI.
The challenge in this part is doing efficient input communication
or even parallel I/O,
since usually the inputs are FASTA files,
which is a text format that need to be parsed sequentially.

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

[1]: http://www.eecs.berkeley.edu/~egeor/sc14_genome.pdf
[2]: https://github.com/ged-lab/khmer/pull/257
[3]: http://metagenomics.anl.gov/metagenomics.cgi?page=MetagenomeProject&project=6368
