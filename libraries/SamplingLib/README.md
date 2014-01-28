SamplingLib
===========

An independant library that implements various sampling methods to samples a data serie (1 dimensional) by approximation, in order to reduce the storate size.
Two methods currently implementes :
- Incremental Sampling : goes from the beginning to the end of the serie and select a point when error goes above threshold
- Max Split Sampling : splits the interval of the data serie by selecting points that have the maximum error with the linear approximation
