# Custom `malloc` and `free` Implementation for MIT's xv6 OS

## Summary

This project involves the implementation of custom versions of the malloc and free memory management functions for MIT's xv6 operating system. The key features of this implementation include:

 - Free List Management: A linked list data structure is used to manage free memory spaces. Each node in this list represents a block of free memory and is stored within the free space itself, optimizing memory usage.

 - Best-Fit Allocation Strategy: The implementation uses a best-fit strategy for memory allocation, minimizing wasted space by selecting the smallest suitable free block for allocation.

 - Efficient Memory Handling: Allocated memory is tracked using a header structure placed before the allocated region, which stores metadata such as the size of the block and a magic number for integrity checks.

 - Memory Coalescence: Adjacent free memory blocks are automatically merged (coalesced) to reduce fragmentation and optimize the use of available memory.

 - Comprehensive Error Handling: The functions include safeguards against invalid memory requests, such as disallowing allocations smaller than 1 byte and imposing an upper limit on allocation size.

This implementation emphasizes efficiency and robustness, making it a valuable learning experience in understanding low-level memory management, pointer arithmetic, and system calls within the xv6 operating system.

## References:

 - Arpaci-Dusseau, RH and AC. 2018. _Operating Systems: Three Easy Pieces._ [Online]. 1st ed. Arpaci-Dusseau Books. [Accessed 26 November 2022]. Available from: https://pages.cs.wisc.edu/~remzi/OSTEP/vm-freespace.pdf
 - Cox, R et al. 2020. _xv6: a simple, Unix-like teaching operating system._ [Online]. 1st ed. MIT PDOS. [Accessed 26 November 2022]. Available from: https://pdos.csail.mit.edu/6.S081/2020/xv6/book-riscv-rev1.pdf
 - Lanza, N T. 2018. _Stack vs Heap. What’s the difference and why should I care?_ 22 April. Medium. [Online]. [Accessed 26 November 2022]. Available from: https://nickolasteixeira.medium.com/stack-vs-heap-whats-the-difference-and-why-should-i-care-5abc78da1a88
 - Programiz. 2022. _Linked list data structure._ [Online]. [Accessed 25 November 2022]. Available from: https://www.programiz.com/dsa/linked-list
