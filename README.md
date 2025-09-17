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
