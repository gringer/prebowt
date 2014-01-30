prebowt
=======

Prefix-array Bowtie-like Transform

A prefix array transform of a DNA sequence, allowing for all possible base ambiguities together with some capability to store quality scores. The transform stores all the *prefixes* of any substring of the sequence in a compressed, sorted
form. This is similar to the Burrows-Wheeler Transform (BWT), which stores the *suffixes* of sequence substrings in a compressed, sorted form.

To increase search speed for large structures, the transform is supplemented by a delta index, which stores the counts of bases (i.e. A/C/G/T/N/$ in the sequence) relative to the expected count at that location based on an assumption that base frequencies are equal (this will have poor performance for non-random sequences). This has a similar structure to the FM-index that is frequently coupled with the BWT structure, essentially using buckets but excluding the need for super-buckets that store explicit counts.
