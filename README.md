# paging-and-related-concepts
This program implements hierarchical paging and dirty bit checkpointing algorithms. It allows allocating and deallocating memory for processes, mapping logical addresses to physical addresses, displaying the page tables, and managing free frames.

## Prerequisites
- C compiler

## Usage
1. Open a terminal or command prompt.
2. Compile the code using the C compiler. For example, if using GCC, run the following command:
gcc -o program program.c

markdown
Copy code
3. Run the program:
./program

less
Copy code

## Menu Options
The program provides the following menu options:

1. Allocate Memory: Allocates memory for a process.
2. Deallocate Memory: Deallocates memory for a process.
3. Show Page Tables: Displays the page tables for all allocated processes.
4. Address Mapping: Maps a logical address to a physical address for a process.
5. Show Free Frames: Displays the list of free frames.
6. Dirty Bit Checkpointing: Performs dirty bit checkpointing for a set of pages.
7. Exit: Exits the program.

## Hierarchical Paging
The hierarchical paging algorithm allows dividing the memory into multiple levels of page tables. Each process has a level 1 table, and each level 1 table can have multiple level 2 tables. Each level 2 table contains a set of page table entries.

The available menu options related to hierarchical paging are:
- Allocate Memory: Allocates memory for a process based on the page size and memory size.
- Deallocate Memory: Deallocates memory for a process and frees the corresponding frames.
- Show Page Tables: Displays the page tables for all allocated processes.
- Address Mapping: Maps a logical address to a physical address for a process.

## Dirty Bit Checkpointing
The dirty bit checkpointing algorithm allows marking pages as dirty or clean. It simulates page references and provides an option to perform a checkpoint, which marks a range of pages as clean.

The available menu options related to dirty bit checkpointing are:
- Dirty Bit Checkpointing: Performs dirty bit checkpointing for a set of pages.
- Exit: Exits the program.

Note: The dirty bit checkpointing algorithm is not directly related to hierarchical paging and is implemented as a separate function.

