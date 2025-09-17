# Bin-packing
The project aims to implement and compare greedy bin-packing heuristics with efficient data structures to find practical solutions for large-scale file storage optimization.


The bin packing problem is applied to backing up a university’s data distributed across files of size ≤ B, stored entirely on disks of capacity B. The goal is to minimize the number of disks used and determine file distribution. Since the problem is NP-hard, the project focuses on efficient greedy heuristics:

- Next-Fit: keep one disk open, add files sequentially, open new disk when needed.

- Worst-Fit: place each file on the least full open disk.

- Best-Fit: place each file on the most full disk that can still fit it.

- First-Fit: place each file on the first open disk (in order of creation) that can fit it.

Files are processed in descending size order to improve packing efficiency. Due to potentially large N (hundreds of thousands of files), the project requires efficient data structures:

No special structure for Next-Fit,
Priority queue for Worst-Fit,
Balanced binary search tree for Best-Fit,
Augmented binary search tree (with disk indices and subtree capacity info) for First-Fit.


 ### Data Structures
 
 Since the universitys les number several hundred thousand, your implementation must be as
 e cient as possible to allow SEGI to obtain a solution in a reasonable amount of time. This
 means that for certain strategies, you will need to use the most appropriate data structures. Some
 ideas are given below for each strategy.
 - Next-Fit. This is the simplest approach. No special structure is required to implement it
 e ciently.

 - Worst-Fit. This solution requires accessing for each new le on disk among the open ones that
 is the least full. It must also be possible to update the empty disk space. This seems to t very
 well with the speci cations of a priority queue.

 - Best-Fit. Here, we must be able to access the disk with the lowest free space that can receive
 the le. This seems possible if you store the open disks in a binary search tree using the size of
 the free disk space as the key (see gure 1 and question 3 of the theoretical analysis). To ensure
 e cient code, however, it is necessary to use a balanced tree implementation.

 - First-Fit. This is the most complicated approach to implement e ciently. It is necessary to
 be able to nd the rst open disk that can contain the le. Therefore, there are two criteria to
 consider: the size of the empty space on the disk and the time at which the disk was opened. One
 possible approach is to use a binary search tree using the disk index as the key in the order it
 was opened. By adding the maximum capacity of the disks in the subtree of which this node is
 the root, to each node of this tree, it is possible to quickly nd the target disk (see gure 2 and
 question 4 of the theoretical analysis).
