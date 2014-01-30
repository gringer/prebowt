### prebowt
###### [Prefix-array Bowtie-like Transform]

This program encodes a prefix array transform of a DNA sequence, allowing for all possible base ambiguities together with some capability to store quality scores. The transform stores all the *prefixes* of any substring of the sequence in a compressed, sorted form. This is similar to the Burrows-Wheeler Transform (BWT), which stores the *suffixes* of sequence substrings in a compressed, sorted form. The transform will be stored as a rope structure (or possibly a zipper), which enables fast dynamic insertion of new sequences.

To increase search speed for large structures, the transform is supplemented by a delta index, which stores the counts of bases (i.e. A/C/G/T in the sequence) relative to the expected count at that location based on an assumption that base frequencies are equal (this will have poor performance for non-random sequences). This has a similar structure to the FM-index that is frequently coupled with the BWT structure, essentially using buckets but excluding the need for super-buckets that store explicit counts. To save a few bits, ambiguous counts and end markers (N/$) are stored separately and do not adjust the counts for the specified bases.

The transform stores an encoded, sorted form of sequence prefixes and can support multiple sequences by sorting identical sequences based on the order of insertion. As a further demonstration of how this works, here is a binary version of the prefix array transform that has two sequences stored in it. Both of these sequences can be extracted (together with their order of insertion) by following rules very similar to those used for BWT:

```
The length of the prebowt sequence '.1001110110001-11001-' is 20
Matrix / expanded view:
----------|1
----------|0
---------0|0
--------00|1
-------100|1
--00101100|1
--------10|0
------0010|1
----100110|1
---0010110|0
---------1|0
-------001|0
------1001|1
-001011001|-
-----00101|1
---1001101|1
-----10011|0
----001011|0
--10011011|1
-100110111|-
```
