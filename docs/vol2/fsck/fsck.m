|make(rp)

|insert(../monk/monk.mac)

|comment(VOL2HEADER)|set_counter(page 581-1)|set_string(page_headers "'''")|set_string(even_headers "'Fsck \(em The UNIX File System Check Program''\f(NIfsck\fP'")|set_string(odd_headers "'\f(NIfsck\fP''Fsck \(em The UNIX File System Check Program'")|set_string(even_footers "_\\nP__UNIX Papers_")|set_string(odd_footers "'Research Tenth Edition''\\nP'")

|environment(S;
	inline off, size -1;)

|environment(text_list;
	list.type fixed, list.pre "\&", list.format 1, list.post "\ \ ",
	list.indent "nnnnnn", list on;

	list off)

|author(name "T. J. Kowalski", initials TJK, location MH, department 11229,
	extension x2771, room 2C-552)

|date(July 1, 1979)

|title(Fsck |sp(minus) The |s(UNIX) File System Check Program)

|keywords(File System Repair)

|begin(abstract)
The |s(UNIX)|sp(registered) File System Check program |i(fsck)
is an interactive file system check and repair program.
|i(Fsck) uses the redundant structural information
in the |s(UNIX) file system
to do several consistency checks.
If an inconsistency is detected,
it is reported to the operator,
who may elect to fix or ignore each inconsistency.
These inconsistencies
result from the permanent interruption of the file system updates,
which are performed every time a file is modified.
|i(Fsck) is frequently able to repair corrupted file systems
using procedures based on the order in which |s(UNIX) honors
these file system update requests.
|p
The purpose of this document
is to describe the normal updating of the file system,
to discuss the possible causes of file system corruption,
and to present the corrective actions carried out by |i(fsck).
Both the program
and the interaction between the program and the operator are described.
|end(abstract)

|titlebox

|style(two_column)

|section(Introduction)

When a |s(UNIX) system|reference(ritchie thompson unix cacm)
is brought up,
a consistency check of the file systems should always be performed.
This precautionary measure
helps to insure a reliable environment for file storage on disk.
If an inconsistency is discovered,
corrective action must be taken.
No changes are made to any file system
by |i(fsck) without prior operator approval.
|p
The purpose of this memo is to dispel the mystique
surrounding file system inconsistencies.
It first describes the updating of the file system
(the calm before the storm)
and then describes file system corruption (the storm).
Finally,
the set of heuristically sound corrective actions
used by |i(fsck)|reference(system iii manual)
(the Coast Guard to the rescue)
is presented.

|section(Update Of The File System)

Every working day hundreds of files are created, modified, and removed.
Every time a file is modified, the |s(UNIX) operating system
performs a series of file system updates.
These updates, when written on disk, yield a consistent file system.
To understand what happens
if a permanent interruption occurs in this sequence,
it is important to know the probable order of update requests.|reference(thompson unix bstj)
Knowing what pieces of information were probably written
to the file system first,
heuristic procedures can be developed to repair a corrupted file system.
|p
There are five types of file system updates.
These involve the super-block,
inodes, indirect blocks,
data blocks (directories and files), and free list blocks.

|subsection(Super-Block)

The super-block contains
part of the free block list,
part of the free i-list,
and information about
the size of the file system,
the size of the i-list,
the count of free blocks,
and the count of free inodes.
|p
The super-block of a mounted file system
(the root file system is always mounted)
is written whenever the file system is unmounted
or a |i(sync) command is issued.

|subsection(Inodes)

An inode contains information about
the type of inode (directory, data, or special),
the number of directory entries linked to the inode,
the list of blocks claimed by the inode,
and the size of the inode.
|p
An inode is written to the file system
when the file associated with the inode is closed.|footnote(All
in-core blocks are also written to the file system on issue of
a |i(sync) system call.)

|subsection(Indirect Blocks)

There are three types of indirect blocks:
single-indirect, double-indirect and triple-indirect.
A single-indirect block contains
a list of some of the block numbers claimed by an inode.
Each of the 128 indirect block entries is a data-block number.
A double-indirect block contains a list of single-indirect block numbers.
A triple-indirect block contains a list of double-indirect block numbers.
|p
Indirect blocks are written to the file system
whenever they have been modified and released|footnote(More precisely,
they are queued for eventual writing.
Physical I/O is deferred until the buffer is needed by UNIX
or a |i(sync) command is issued.)
by the operating system.

|subsection(Data Blocks)

A data block may contain file information
or directory entries.
Each directory entry consists of a file name
and an inode number.
|p
Data blocks are written to the file system
whenever they have been modified and released by the operating system.

|subsection(First Free List Block)

The super-block contains the first free list block.
The free list blocks
are a list of all blocks that are not allocated to the super-block,
inodes, indirect blocks, or data blocks.
Each free list block contains
a count of the number of entries in this free list block,
a pointer to the next free list block,
and a partial list of free blocks in the file system.
|p
Free list blocks are written to the file system
whenever they have been modified and released by the operating system.

|section(Corruption Of The File System)

A file system can become corrupted in a variety of ways.
The most common problems
are improper shutdown procedures and hardware failures.

|subsection(Improper System Shutdown and Startup)

File systems may become corrupted when proper shutdown procedures
are not observed, e.g.,
forgetting to |i(sync) the system before halting the |s(CPU),
physically write-protecting a mounted file system,
or taking a mounted file system off-line.
|p
File systems may become further corrupted if proper startup procedures
are not observed, e.g.,
not checking a file system for inconsistencies,
and not repairing inconsistencies.
Allowing a corrupted file system to be used (and, thus, to be modified further)
can be disastrous.

|subsection(Hardware Failure)

Any piece of hardware can fail at any time.
Failures can be as subtle as a bad block on a disk pack,
or as obvious as a non-functional disk-controller.

|section(Detection And Correction Of Corruption)

A quiescent|footnote(I.e., unmounted and not being written on.)
file system may be checked for structural integrity
by performing consistency checks on the redundant data
intrinsic to a file system.
The redundant data is either read from the file system
or computed from other known values.
A quiescent state is important during the checking of a file system
because of the multi-pass nature of the |i(fsck) program.
|p
When an inconsistency is discovered
|i(fsck) reports the inconsistency
and requests the operator to choose a corrective action.
|p
Discussed in this section are how to discover inconsistencies
and possible corrective actions
for the super-block, the inodes, the indirect blocks,
the data blocks containing directory entries, and the free list blocks.
These corrective actions
can be performed interactively by the |i(fsck) command
under control of the operator.

|subsection(Super-Block)

The most common corrupted item is the super-block.
The super-block is prone to corruption
because every change to the file system's blocks or inodes
modifies the super-block.
|p
The super-block and its associated parts
are most often corrupted when the computer is halted
and the last command involving output to the file system
was not a |i(sync) command.
|p
The super-block can be checked for inconsistencies involving file system size,
i-list size,
free block list,
free block count,
and the free inode count.

|subsubsection(File system size and i-list size.)
|save(File system size and i-list size.)

The file system size must be larger than the number of blocks
used by the super-block
and the number of blocks used by the list of inodes.
The number of inodes must be less than 65,535.
The file system size and i-list size
are critical pieces of information to the |i(fsck) program.
While there is no way to check these sizes,
|i(fsck) can check that they are within reasonable bounds.
All other checks of the file system
depend on the correctness of these sizes.

|subsubsection(Free block list.)
|save(Free block list.)

The free block list starts in the super-block
and continues through the free list blocks of the file system.
Each free list block can be checked
for list count out of range,
for block numbers out of range,
and for blocks already allocated within the file system.
A check is made to see that all the blocks in the file system were found.
|p
The first free block list is in the super-block.
|i(Fsck) checks the list count
for a value of less than zero or greater than fifty.
It also checks each block number
for a value of less than the first data block in the file system
or greater than the last block in the file system.
Then it compares each block number to a list of already allocated blocks.
If the free list block pointer is non-zero,
the next free list block is read in and the process is repeated.
|p
When all the blocks have been accounted for,
a check is made to see if the number of blocks used by the free block list
plus the number of blocks claimed by the inodes
equals the total number of blocks in the file system.
|p
If anything is wrong with the free block list,
then |i(fsck) may rebuild it,
excluding all blocks in the list of allocated blocks.

|subsubsection(Free block count.)
|save(Free block count.)

The super-block contains a count of the total number of free blocks
within the file system.
|i(Fsck) compares this count to the number of free blocks it found
within the file system.
If they don't agree,
then |i(fsck) may replace the count in the super-block
by the real free block count.

|subsubsection(Free inode count.)
|save(Free inode count.)

The super-block contains a count of the total number of free inodes
within the file system.
|i(Fsck) compares this count to the number of free inodes it found
within the file system.
If they don't agree,
then |i(fsck)
may replace the count in the super-block by the real free inode count.

|subsection(Inodes)

An individual inode is not as likely to be corrupted as the super-block.
However,
because of the great number of active inodes,
corruption in the i-list is almost as likely as corruption
in the super-block.
|p
The list of inodes is checked sequentially starting with inode 1
(there is no inode 0)
and going to the last inode in the file system.
Each inode can be checked for
inconsistencies involving format and type,
link count,
duplicate blocks,
bad blocks,
and inode size.

|subsubsection(Format and type.)
|save(Format and type.)

Each inode contains a mode word.
This mode word describes the type and state of the inode.
Inodes may be one of four types:
regular inode, directory inode, special block inode,
and special character inode.
If an inode is not one of these types, then the inode has an illegal type.
Inodes may be found in one of three states:
unallocated, allocated, and neither unallocated nor allocated.
This last state suggests an incorrectly formatted inode.
Incorrect format may result
if bad data is written into the i-list through,
for example,
a hardware failure.
The only possible corrective action for |i(fsck) is clearing the inode.

|subsubsection(Link count.)
|save(Link count.)

Contained in each inode is a count of the total number of directory entries
linked to the inode.
|p
|i(Fsck) verifies the link count of each inode
by traversing down the total directory structure,
starting from the root directory,
and calculating a real link count for each inode.
|p
If the stored link count is non-zero and the real link count is zero,
it means that no directory entry appears for the inode.
If the stored and real link counts are non-zero and unequal,
a directory entry may have been added or removed
without the inode being updated.
|p
If the stored link count is non-zero and the real link count is zero,
|i(fsck) may link the disconnected file to the |cw(lost+found) directory.
If the stored and real link counts are non-zero and unequal,
|i(fsck) may replace the stored link count by the real link count.

|subsubsection(Duplicate blocks)
|save(Duplicate blocks)

Contained in each inode is a list or pointers to lists (indirect blocks)
of all the blocks claimed by the inode.
|p
|i(Fsck) compares each block number claimed by an inode
to a list of already allocated blocks.
If a block number is already claimed by another inode,
the block number is added to a list of duplicate blocks.
Otherwise,
the list of allocated blocks is updated to include the block number.
If there are any duplicate blocks,
|i(fsck) will pass over part of the i-list a second time
to find the inode of the duplicated block,
because without examining the files
associated with these inodes for correct content,
there is not enough information available to decide what inode is corrupted
and should be cleared.
Usually the inode with the earliest modify time is incorrect
and should be cleared.
|p
This condition can occur
when a file system contains blocks claimed by both the free block list
and by other parts of the file system.
|p
If there is a large number of duplicate blocks in an inode,
this may be because an indirect block is not being written to the file system.
|p
|i(Fsck) will prompt the operator to clear both inodes.

|subsubsection(Bad blocks)
|save(Bad blocks)

Contained in each inode
is a list or pointer to lists of all the blocks claimed by the inode.
|p
|i(Fsck) checks each block number claimed by an inode
for a value lower than that of the first data block,
or greater than the last block in the file system.
If the block number is outside this range,
the block number is a bad block number.
|p
If there is a large number of bad blocks in an inode,
this may be because an indirect block is not being written to the file system.
|p
|i(Fsck) will prompt the operator to clear both inodes.

|subsubsection(Size checks.)
|save(Size checks.)

Each inode contains a thirty-two bit (four-byte) size field.
This size shows the number of characters in the file
associated with the inode.
This size can be checked for inconsistencies,
e.g.,
directory sizes that are not a multiple of sixteen characters,
or a mismatch between the number of blocks shown by the inode size
and the number really in use.
|p
A directory inode within the |s(UNIX) file system
has the directory bit set in the inode mode word.
The directory size must be a multiple of sixteen
because a directory entry contains sixteen bytes
(two bytes for the inode number
and fourteen bytes for the file or directory name).
|p
|i(Fsck) will warn of directory misalignment.
This is only a warning
because not enough information can be gathered to correct the misalignment.
|p
A rough check of the consistency of the size field of an inode
can be performed by using it to compute
the number of blocks that should be associated with the inode
and comparing it to the real number of blocks claimed by the inode.
|p
|i(Fsck) calculates the number of blocks that should be in an inode
by dividing the number of characters in an inode
by the number of characters per block (512)
and rounding up.
|i(Fsck) adds one block for each indirect block associated with the inode.
If the real number of blocks does not match the computed number of blocks,
|i(fsck) will warn of a possible file-size error.
This is only a warning
because |s(UNIX) does not fill in blocks in files created in random order.

|subsection(Indirect Blocks)

Indirect blocks are owned by an inode.
Therefore,
inconsistencies in indirect blocks directly affect the inode that owns them.
|p
Inconsistencies that can be checked
are blocks already claimed by another inode
and block numbers outside the range of the file system.
|p
For a discussion of detection and correction
of the inconsistencies associated with indirect blocks,
apply iteratively Sections |remember(Duplicate blocks)
and |remember(Bad blocks) to each level of indirect blocks.

|subsection(Data Blocks)
|save(Data Blocks)

The two types of data blocks are
plain data blocks and directory data blocks.
Plain data blocks contain the information stored in a file.
Directory data blocks contain directory entries.
|i(Fsck) does not attempt to check
the validity of the contents of a plain data block.
|p
Each directory data block can be checked for inconsistencies involving
directory inode numbers pointing to unallocated inodes,
directory inode numbers greater than the number of inodes in the file system,
incorrect directory inode numbers for ``|b(.)'' and ``|b(..)'',
and directories that are disconnected from the file system.
|p
If a directory entry inode number points to an unallocated inode,
then |i(fsck) may remove that directory entry.
This condition occurs when
the data blocks containing the directory entries
were modified and written to the file system
but the corresponding inode was not.
|p
If a directory entry inode number is pointing beyond the end of the i-list,
|i(fsck) may remove that directory entry.
This condition occurs if bad data is written into a directory data block.
|p
The directory inode number entry for ``|b(.)''
should be the first entry in the directory data block.
Its value should be equal to the inode number for the directory data block.
|p
The directory inode number entry for ``|b(..)''
should be the second entry in the directory data block.
Its value should be equal to
the inode number for the parent of the directory entry
(or the inode number of the directory data block
if the directory is the root directory).
|p
If the directory inode numbers are incorrect,
|i(fsck) may replace them by the correct values.
|p
|i(Fsck) checks the general connectivity of the file system.
If directories are found not to be linked into the file system,
|i(fsck) will link the directory back into the file system
in the |cw(lost+found) directory.
This condition occurs when inodes are written to the file system
but the corresponding directory data blocks are not.

|subsection(Free List Blocks)

Free list blocks are owned by the super-block.
Therefore,
inconsistencies in free list blocks directly affect the super-block.
|p
Inconsistencies that can be checked are
list count out of range,
block numbers out of range,
and blocks already associated with the file system.
|p
For a discussion of detection and correction of the inconsistencies
associated with free list blocks, see Section |remember(Free block list.).

|section(Acknowledgements)

I would like to thank Larry A. Wehr
for advice that led to the first version of |i(fsck),
and Rick B. Brandt for adapting |i(fsck) to |s(UNIX).

|reference_placement

|style(one_column)
.ne 1i
|appendix(FSCK ERROR CONDITIONS)

|style(two_column)

|section(Conventions)
...........

|i(Fsck) is a multi-pass file system check program.
Each file system pass invokes a different |i(Phase) of the |i(fsck) program.
After the initial setup,
|i(fsck) performs successive |i(Phases) over each file system,
checking blocks and sizes,
path-names,
connectivity,
reference counts,
and the free block list
(possibly rebuilding it),
and performs some cleanup.
|p
When an inconsistency is detected,
|i(fsck) reports the error condition to the operator.
If a response is required,
|i(fsck) prints a prompt message and waits for a response.
This appendix explains the meaning of each error condition,
the possible responses,
and the related error conditions.
|p
The error conditions are organized by the |i(Phase) of the |i(fsck) program
in which they can occur.
The error conditions that may occur in more than one Phase
will be discussed under initialization.

|section(Initialization)

Before a file system check can be performed,
certain tables have to be set up and certain files opened.
This section concerns itself with the opening of files
and the initialization of tables.
It lists error conditions resulting from
command line options,
memory requests,
opening of files,
status of files,
file system size checks,
and creation of the scratch file.

|unnumbered_subsection{|cw[C option?]}

|cw(C) is not a legal option to |i(fsck);
legal options are
|cw(-y),
|cw(-n),
|cw(-s),
|cw(-S),
and
|cw(-t).
|i(Fsck) stops on this error condition.
See the |i(fsck)(8) for further detail.

|unnumbered_subsection{|cw[Bad -t option]}

The |cw(-t) option is not followed by a file name.
|i(Fsck) stops on this error condition.
See the |i(fsck)(8) for further detail.

|unnumbered_subsection{|cw[Invalid -s argument, defaults assumed]}

The |cw(-s) option is not suffixed by 3, 4,
or blocks-per-cylinder:blocks-to-skip.
|i(Fsck) assumes a default value of 400 blocks-per-cylinder
and 9 blocks-to-skip.
See the |i(fsck)(8) for more details.

|unnumbered_subsection{|cw[Incompatible options: -n and -s]}

It is not possible to salvage the free block list
without modifying the file system.
|i(Fsck) stops on this error condition.
See the |i(fsck)(8) for further detail.

|unnumbered_subsection{|cw[Can't get memory]}

|i(Fsck)'s request for memory for its virtual memory tables failed.
This should never happen.
|i(Fsck) stops on this error condition.
See a guru.

|unnumbered_subsection{|cw[Can't open checklist file: F]}

The default file system checklist file |b(F) (usually |cw(/etc/checklist))
cannot be opened for reading.
|i(Fsck) stops on this error condition.
Check access modes of |b(F).

|unnumbered_subsection{|cw[Can't stat root]}

|i(Fsck)'s request for statistics about the root directory ``/'' failed.
This should never happen.
|i(Fsck) stops on this error condition.
See a guru.

|unnumbered_subsection{|cw[Can't stat F]}

|i(Fsck)'s request for statistics about the file system |b(F) failed.
It ignores this file system and continues checking the next file system given.
Check access modes of |b(F).

|unnumbered_subsection{|cw[F is not a block or character device]}

You have given |i(fsck) a regular file name by mistake.
It ignores this file system and continues checking the next file system given.
Check file type of |b(F).

|unnumbered_subsection{|cw[Can't open F]}

The file system |b(F) cannot be opened for reading.
It ignores this file system and continues checking the next file system given.
Check access modes of |b(F).

|unnumbered_subsection{|cw[Size check: fsize X isize Y]}

More blocks are used for the i-list |b(Y)
than there are blocks in the file system |b(X),
or there are more than 65,535 inodes in the file system.
It ignores this file system and continues checking the next file system given.
See Section |remember(File system size and i-list size.).

|unnumbered_subsection{|cw[Can't create F]}

|i(Fsck)'s request to create a scratch file |b(F) failed.
It ignores this file system and continues checking the next file system given.
Check access modes of |b(F).

|unnumbered_subsection{|cw[CAN NOT SEEK: BLK B (CONTINUE)]}

|i(Fsck)'s request for moving to a specified block number |b(B)
in the file system failed.
This should never happen.
See a guru.
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) attempt to continue the file system check.
Often,
however the problem will persist.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.
If the block was part of the virtual memory buffer cache,
|i(fsck) will stop with the message ``Fatal I/O error''.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[CAN NOT READ: BLK B (CONTINUE)]}

|i(Fsck)'s request for reading a specified block number |b(B)
in the file system failed.
This should never happen.
See a guru.
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) attempt to continue the file system check.
Often,
however,
the problem will persist.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.
If the block was part of the virtual memory buffer cache,
|i(fsck) will stop with the message ``Fatal I/O error''.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[CAN NOT WRITE: BLK B (CONTINUE)]}
|i(Fsck)'s request for writing a specified block number |b(B)
in the file system failed.
The disk is write-protected.
See a guru.
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) attempt to continue the file system check.
Often,
however,
the problem will persist.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.
If the block was part of the virtual memory buffer cache,
|i(fsck) will stop with the message ``Fatal I/O error''.

|S(|item(NO)) stop the program.

|end(text_list)

|section(Phase 1: Check blocks and sizes)

This phase concerns itself with the i-list.
This section lists error conditions resulting from
checking inode types,
setting up the zero-link-count table,
examining inode block numbers for bad or duplicate blocks,
checking inode size,
and checking inode format.

|unnumbered_subsection{|cw[UNKNOWN FILE TYPE I=I (CLEAR)]}

The mode word of the inode |b(I) shows that the inode is not a
special character inode,
regular inode,
or directory inode.
See Section |remember(Format and type.).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.
This will always invoke the |s(UNALLOCATED) error condition in Phase 2
for each directory entry pointing to this inode.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[LINK COUNT TABLE OVERFLOW (CONTINUE)]}

An internal table for |i(fsck)
containing allocated inodes with a link count of zero has no more room.
Recompile |i(fsck) with a larger value of |s(MAXLNCNT).
|p
Possible responses
to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) continue with the program.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.
If another allocated inode with a zero link count is found,
this error condition is repeated.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[B BAD I=I]}

Inode |b(I) contains block number |b(B) with a number lower than
the number of the first data block in the file system
or greater than the number of the last block in the file system.
This error condition
may invoke the |s(EXCESSIVE) |s(BAD) |s(BLKS) error condition in Phase 1
if inode |b(I) has too many block numbers outside the file system range.
This error condition will always invoke the |s(BAD)/|s(DUP) error condition
in Phase 2 and Phase 4.
See Section |remember(Bad blocks).

|unnumbered_subsection{|cw[EXCESSIVE BAD BLKS I=I (CONTINUE)]}

There is more than a tolerable number (usually 10) of blocks
with a number lower than the number of the first data block in the file system
or greater than the number of last block in the file system
associated with inode |b(I).
See Section |remember(Bad blocks).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) ignore the rest of the blocks in this inode
and continue checking with the next inode in the file system.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[B DUP I=I]}

Inode |b(I) contains block number |b(B),
which is already claimed by another inode.
This error condition
may invoke the |s(EXCESSIVE) |s(DUP) |s(BLKS) error condition in Phase 1
if inode |b(I) has too many block numbers claimed by other inodes.
This error condition will always invoke Phase 1b
and the |s(BAD)/|s(DUP) error condition in Phase 2 and Phase 4.
See Section |remember(Duplicate blocks).

|unnumbered_subsection{|cw[EXCESSIVE DUP BLKS I=I (CONTINUE)]}

There is more than a tolerable number (usually 10) of blocks
claimed by other inodes.
See Section |remember(Duplicate blocks).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) ignore the rest of the blocks in this inode
and continue checking with the next inode in the file system.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[DUP TABLE OVERFLOW (CONTINUE)]}

An internal table in |i(fsck)
containing duplicate block numbers has no more room.
Recompile |i(fsck) with a larger value of |s(DUPTBLSIZE).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) continue with the program.
This error condition will not allow a complete check of the file system.
A second run of |i(fsck) should be made to re-check this file system.
If another duplicate block is found, this error condition will repeat.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[POSSIBLE FILE SIZE ERROR I=I]}

The inode |b(I) size
does not match the real number of blocks used by the inode.
This is only a warning.
See Section |remember(Size checks.).

|unnumbered_subsection{|cw[DIRECTORY MISALIGNED I=I]}

The size of a directory inode
is not a multiple of the size of a directory entry (usually 16).
This is only a warning.
See Section |remember(Size checks.).

|unnumbered_subsection{|cw[PARTIALLY ALLOCATED INODE I=I (CLEAR)]}

Inode |b(I) is neither allocated nor unallocated.
See Section |remember(Format and type.).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|section(Phase 1b: Rescan for more dups)

When a duplicate block is found in Phase 1,
the file system is rescanned to find the inode
that previously claimed that block.
This section describes when the duplicate block is found.

|unnumbered_subsection{|cw[B DUP I=I]}

Inode |b(I) contains block number |b(B),
which is already claimed by another inode.
This error will always invoke the |s(BAD)/|s(DUP) error condition
in Phase 2.
You can determine what inodes have overlapping blocks
by examining this error condition
and the |s(DUP) error condition in Phase 1.
See Section |remember(Duplicate blocks).

.......
|section(Phase 2: Check path-names)

This phase concerns itself with removing directory entries pointing to
flawed inodes detected in Phase 1 and Phase 1b.
This section lists error conditions resulting from
root inode mode and status,
directory inode pointers in range,
and directory entries pointing to bad inodes.

|unnumbered_subsection{|cw[ROOT INODE UNALLOCATED. TERMINATING.]}

The root inode (usually inode number 2) has no allocate mode bits.
This should never happen.
The program will stop.
See Section |remember(Format and type.).

|unnumbered_subsection{|cw[ROOT INODE NOT DIRECTORY (FIX)]}

The root inode (usually inode number 2) is not directory inode type.
See Section |remember(Format and type.).
|p
Possible responses to the |s(FIX) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) make the root inode's type be a directory.
If the root inode's data blocks are not directory blocks,
a large number of error conditions will be produced.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[DUPS/BAD IN ROOT INODE (CONTINUE)]}

Phase 1 or Phase 1b has found duplicate blocks
or bad blocks in the root inode (usually inode number 2) for the file system.
See Sections |remember(Duplicate blocks) and |remember(Bad blocks).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) ignore the |s(DUPS)/|s(BAD) error condition in the root inode and
attempt to continue the file system check.
If the root inode is not correct,
then this action may result in a large number of other error conditions.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[I OUT OF RANGE I=I NAME=F (REMOVE)]}

A directory entry |b(F) has an inode number |b(I)
that is greater than the end of the i-list.
See Section |remember(Data Blocks).
|p
Possible responses to the |s(REMOVE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) the directory entry |b(F) will be removed.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[UNALLOCATED I=I OWNER=O MODE=M SIZE=S MTIME=T NAME=F (REMOVE)]}

A directory entry |b(F) has an inode |b(I) without allocate mode bits.
The owner |b(O), mode |b(M), size |b(S), modify time |b(T),
and file name |b(F) are printed.
See Section |remember(Data Blocks).
|p
Possible responses to the |s(REMOVE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) the directory entry |b(F) will be removed.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[DUP/BAD I=I OWNER=O MODE=M SIZE=S MTIME=T DIR=F (REMOVE)]}

Phase 1 or Phase 1b has found duplicate blocks or bad blocks
associated with directory entry/inode |b(F)/|b(I).
The owner |b(O), mode |b(M), size |b(S), modify time |b(T),
and directory name |b(F) are printed.
See Sections |remember(Duplicate blocks) and |remember(Bad blocks).
|p
Possible responses to the |s(REMOVE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) the directory entry |b(F) will be removed.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[DUP/BAD I=I OWNER=O MODE=M SIZE=S MTIME=T FILE=F (REMOVE)]}

Phase 1 or Phase 1b has found duplicate blocks or bad blocks
associated with directory entry/inode |b(F)/|b(I).
The owner |b(O), mode |b(M), size |b(S), modify time |b(T),
and file name |b(F) are printed.
See Sections |remember(Duplicate blocks) and |remember(Bad blocks).
|p
Possible responses to the |s(REMOVE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) the directory entry |b(F) will be removed.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|section(Phase 3: Check connectivity)

This phase concerns itself with the directory connectivity seen in Phase 2.
This section lists error conditions resulting from
unreferenced directories
and missing or full |cw(lost+found) directories.

|unnumbered_subsection{|cw[UNREF DIR I=I OWNER=O MODE=M SIZE=S MTIME=T (RECONNECT)]}

The directory inode |b(I)
was not connected to a directory entry when the file system was traversed.
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of directory inode |b(I) are printed.
See Sections |remember(Data Blocks) and |remember(Link count.).
|p
Possible responses to the |s(RECONNECT) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) reconnect directory inode |b(I) to the file system
in the directory for lost files (usually |cw(lost+found)).
This action may invoke the |cw(lost+found) error condition in Phase 3
if there are problems connecting directory inode |b(I) to |cw(lost+found).
If the link is successful,
this action may also invoke the |s(CONNECTED) error condition in Phase 3.

|S(|item(NO)) ignore this error condition.
The |s(UNREF) error condition in Phase 4 will always occur as a result.

|end(text_list)

|unnumbered_subsection{|cw[SORRY. NO lost+found DIRECTORY]}

There is no |cw(lost+found) directory in the root directory of the file system;
|i(fsck) ignores the request to link a directory in |cw(lost+found).
The |s(UNREF) error condition in Phase 4 will always occur as a result.
Check access modes of |cw(lost+found).
See |i(fsck)(1M) manual entry for further detail.

|unnumbered_subsection{|cw[SORRY. NO SPACE IN lost+found DIRECTORY]}

There is no space to add another entry to the |cw(lost+found) directory
in the root directory of the file system;
|i(fsck) ignores the request to link a directory in |cw(lost+found).
The |s(UNREF) error condition in Phase 4 will always occur as a result.
Clean out unnecessary entries in |cw(lost+found)
or make |cw(lost+found) larger.
See |i(fsck)(1M) manual entry for further detail.

|unnumbered_subsection{|cw[DIR I=I1 CONNECTED. PARENT WAS I=I2]}

This is an advisory message showing that a directory inode |b(I1)
was successfully connected to the |cw(lost+found) directory.
The parent inode |b(I2) of the directory inode |b(I1)
is replaced by the inode number of the |cw(lost+found) directory.
See Sections |remember(Data Blocks) and |remember(Link count.).

|section(Phase 4: Check reference counts)

This phase concerns itself with the link count information
seen in Phase 2 and Phase 3.
This section lists error conditions resulting from
unreferenced files,
missing or full |cw(lost+found) directory,
incorrect link counts for files, directories, or special files,
unreferenced files and directories,
bad and duplicate blocks in files and directories,
and incorrect total free inode counts.

|unnumbered_subsection{|cw[UNREF FILE I=I OWNER=O MODE=M SIZE=S MTIME=T (RECONNECT)]}

Inode |b(I)
was not connected to a directory entry when the file system was traversed.
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of inode |b(I) are printed.
See Section |remember(Link count.).
|p
Possible responses to the |s(RECONNECT) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) reconnect inode |b(I) to the file system
in the directory for lost files (usually |cw(lost+found)).
This action may invoke the |cw(lost+found) error condition in Phase 4
if there are problems connecting inode |b(I) to |cw(lost+found).

|S(|item(NO)) ignore this error condition.
This will always invoke the |s(CLEAR) error condition in Phase 4.

|end(text_list)

|unnumbered_subsection{|cw[SORRY. NO lost+found DIRECTORY]}

There is no |cw(lost+found) directory in the root directory of the file system;
|i(fsck) ignores the request to link a file in |cw(lost+found).
This will always invoke the |s(CLEAR) error condition in Phase 4.
Check access modes of |cw(lost+found).

|unnumbered_subsection{|cw[SORRY. NO SPACE IN lost+found DIRECTORY]}

There is no space to add another entry to the |cw(lost+found) directory
in the root directory of the file system;
|i(fsck) ignores the request to link a file in |cw(lost+found).
This will always invoke the |s(CLEAR) error condition in Phase 4.
Check size and contents of |cw(lost+found).

|unnumbered_subsection{|cw[(CLEAR)]}

The inode mentioned in the immediately previous error condition
cannot be reconnected.
See Section |remember(Link count.).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate the inode
mentioned in the preceding error condition by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[LINK COUNT FILE I=I OWNER=O MODE=M SIZE=S MTIME=T COUNT=X SHOULD BE Y (ADJUST)]}

The link count for inode |b(I), which is a file, is |b(X) but should be |b(Y).
The owner |b(O), mode |b(M), size |b(S), and modify time |b(T) are printed.
See Section |remember(Link count.).
|p
Possible responses to the |s(ADJUST) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the link count of file inode |b(I) with |b(Y).

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[LINK COUNT DIR I=I OWNER=O MODE=M SIZE=S MTIME=T COUNT=X SHOULD BE Y (ADJUST)]}

The link count for inode |b(I),
which is a directory, is |b(X) but should be |b(Y).
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of directory inode |b(I) are printed.
See Section |remember(Link count.).
|p
Possible responses to the |s(ADJUST) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the link count of directory inode |b(I) with |b(Y).

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[LINK COUNT F I=I OWNER=O MODE=M SIZE=S MTIME=T COUNT=X SHOULD BE Y (ADJUST)]}

The link count for inode |b(I) is |b(X) but should be |b(Y).
The name |b(F), owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) are printed.
See Section |remember(Link count.).
|p
Possible responses to the |s(ADJUST) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the link count of inode |b(I) with |b(Y).

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[UNREF FILE I=I OWNER=O MODE=M SIZE=S MTIME=T (CLEAR)]}

Inode |b(I), which is a file,
was not connected to a directory entry when the file system was traversed.
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of inode |b(I) are printed.
See Sections |remember(Link count.) and |remember(Data Blocks).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[UNREF DIR I=I OWNER=O MODE=M SIZE=S MTIME=T (CLEAR)]}

Inode |b(I), which is a directory,
was not connected to a directory entry when the file system was traversed.
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of inode |b(I) are printed.
See Sections |remember(Link count.) and |remember(Data Blocks).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[BAD/DUP FILE I=I OWNER=O MODE=M SIZE=S MTIME=T (CLEAR)]}

Phase 1 or Phase 1b has found duplicate blocks or bad blocks
associated with file inode |b(I).
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of inode |b(I) are printed.
See Sections |remember(Duplicate blocks) and |remember(Bad blocks).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[BAD/DUP DIR I=I OWNER=O MODE=M SIZE=S MTIME=T (CLEAR)]}

Phase 1 or Phase 1b has found duplicate blocks or bad blocks
associated with directory inode |b(I).
The owner |b(O), mode |b(M), size |b(S),
and modify time |b(T) of inode |b(I) are printed.
See Sections |remember(Duplicate blocks) and |remember(Bad blocks).
|p
Possible responses to the |s(CLEAR) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) de-allocate inode |b(I) by zeroing its contents.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[FREE INODE COUNT WRONG IN SUPERBLK (FIX)]}

The real count of the free inodes
does not match the count in the super-block of the file system.
See Section |remember(Free inode count.).
|p
Possible responses to the |s(FIX) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the count in the super-block by the real count.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|section(Phase 5: Check free list)

This phase concerns itself with the free block list.
This section lists error conditions resulting from
bad blocks in the free block list,
bad free blocks count,
duplicate blocks in the free block list,
unused blocks from the file system not in the free block list,
and an incorrect total free block count.

|unnumbered_subsection{|cw[EXCESSIVE BAD BLKS IN FREE LIST (CONTINUE)]}

The free block list contains more than a tolerable number (usually 10)
of blocks with a value less than the first data block in the file system
or greater than the last block in the file system.
See Sections |remember(Free block list.) and |remember(Bad blocks).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) ignore the rest of the free block list
and continue the execution of |i(fsck).
This error condition will always invoke
the |s(BAD) |s(BLKS) |s(IN) |s(FREE) |s(LIST) error condition in Phase 5.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[EXCESSIVE DUP BLKS IN FREE LIST (CONTINUE)]}

The free block list contains more than a tolerable number (usually 10)
of blocks claimed by inodes or earlier parts of the free block list.
See Sections |remember(Free block list.) and |remember(Duplicate blocks).
|p
Possible responses to the |s(CONTINUE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) ignore the rest of the free block list
and continue the execution of |i(fsck).
This error condition will always invoke
the |s(DUP) |s(BLKS) |s(IN) |s(FREE) |s(LIST) error condition in Phase 5.

|S(|item(NO)) stop the program.

|end(text_list)

|unnumbered_subsection{|cw[BAD FREEBLK COUNT]}

The count of free blocks in a free list block is greater than 50
or less than zero.
This error condition will always invoke
the |s(BAD) |s(FREE) |s(LIST) condition in Phase 5.
See Section |remember(Free block list.).

|unnumbered_subsection{|cw[X BAD BLKS IN FREE LIST]}

|b(X) blocks in the free block list
have a block number lower than the first data block in the file system
or greater than the last block in the file system.
This error condition will always invoke
the |s(BAD) |s(FREE) |s(LIST) condition in Phase 5.
See Sections |remember(Free block list.) and |remember(Bad blocks).

|unnumbered_subsection{|cw[X DUP BLKS IN FREE LIST]}

|b(X) blocks claimed by inodes or earlier parts of the free list block
were found in the free block list.
This error condition will always invoke
the |s(BAD) |s(FREE) |s(LIST) condition in Phase 5.
See Sections |remember(Free block list.) and |remember(Duplicate blocks).

|unnumbered_subsection{|cw[X BLK(S) MISSING]}

|b(X) blocks unused by the file system were not found in the free block list.
This error condition will always invoke
the |s(BAD) |s(FREE) |s(LIST) condition in Phase 5.
See Section |remember(Free block list.).

|unnumbered_subsection{|cw[FREE BLK COUNT WRONG IN SUPERBLOCK (FIX)]}

The real count of free blocks
does not match the count in the super-block of the file system.
See Section |remember(Free block count.).
|p
Possible responses to the |s(FIX) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the count in the super-block by the real count.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|unnumbered_subsection{|cw[BAD FREE LIST (SALVAGE)]}

Phase 5 has found
bad blocks in the free block list,
duplicate blocks in the free block list,
or blocks missing from the file system.
See Sections |remember(Free block list.), |remember(Duplicate blocks),
and |remember(Bad blocks).
|p
Possible responses to the |s(SALVAGE) prompt are:

|blank_space(1)
|begin(text_list)

|S(|item(YES)) replace the current free block list with a new free block list.
The new free block list will be ordered to reduce time
spent by the disk waiting to rotate into position.

|S(|item(NO)) ignore this error condition.

|end(text_list)

|section(Phase 6: Salvage free list)

This phase concerns itself with the free block list reconstruction.
This section lists error conditions resulting from
the blocks-to-skip and blocks-per-cylinder values.

|unnumbered_subsection{|cw[Default free block list spacing assumed]}

This is an advisory message
showing that the blocks-to-skip is greater than the blocks-per-cylinder,
the blocks-to-skip is less than one,
the blocks-per-cylinder is less than one,
or the blocks-per-cylinder is greater than 500.
The default values of 9 blocks-to-skip and 400 blocks-per-cylinder are used.
See the |i(fsck)(1M) manual entry for further detail.

|section(Cleanup)

Once a file system has been checked, a few cleanup functions are performed.
This section lists advisory messages about
the file system
and its modified status.

|unnumbered_subsection{|cw[X files Y blocks Z free]}

This is an advisory message showing that the file system checked
contained |b(X) files
using |b(Y) blocks
leaving |b(Z) blocks free in the file system.

|unnumbered_subsection{|cw[***** BOOT UNIX (NO SYNC!) *****]}

This is an advisory message showing that a mounted file system
or the root file system has been modified by |i(fsck).
If |s(UNIX) is not rebooted immediately,
the work done by |i(fsck)
may be undone by the in-core copies of tables kept by |s(UNIX).

|unnumbered_subsection{|cw[***** FILE SYSTEM WAS MODIFIED *****]}

This is an advisory message showing that the current file system
was modified by |i(fsck).
If this file system is mounted or is the current root file system,
|i(fsck) should be halted and |s(UNIX) rebooted.
If |s(UNIX) is not rebooted immediately,
the work done by |i(fsck)
may be undone by the in-core copies of tables kept by |s(UNIX).
