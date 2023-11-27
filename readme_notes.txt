** Virtual Memory Key Terms **
----------------------------------------------------------
Page Table - Tracks translations of 'Virtual Addresses' to 'Physical Addresses' for each program.

Page Fault - 

Page Table Cache - 

Translation Lookup(Look-aside) Buffer (TLB) - A method for faster page table translations. 

Virtually Indexed, Physically Tagged (VIPT):
  1) Data in cached is 'indexed' by the 'virtual address'
  2) But 'tagged' by 'physical address'
  3) We only get a 'hit' if tag matches physical address, but we can start looking for virtual address.


** Virtual Memory General Notes **
---------------------------------------------------------
For each memory access with virtual memory, we have to:
  1) 'access the page table', 
  2) 'translate the address', 
  3) 'access the data in RAM'. 

CPU->Memory Transaction Speeds:
  1) Cache - Fastest
  2) RAM - Fast
  3) Disk - Slowest 

TLB notes: 
  1) You can make TLB pages bigger.

~ In Virtual Memory, 'Virtual Addresses' are mapped to 'Physical Address' in RAM.
~ A 'Virtual Address' consists of a 'Virtual Page Number' and 'Page Offset'. 
~ A 'Page Table' must remain in 'Main Memory' at all times, otherwise we won't be able to access anything.
~ If a 'Page Table Entry' does not exist in the 'TLB', it is loaded from 'Memory' into the 'TLB'. 
~ Each program needs its own 'Page Table'. 
~ With a 'Virtual Cache', TLB lookups only occurs when we miss in cache.
~ Two programs cannot share a 'Virtual Cache'. 
~ 'Virtual Memory' adds a level of 'indirection' between 'Virtual Program Addresses (VA)' and 'Physical RAM Addresses (PA)'. This allows us to 'map' memory to disk ("unlimited" memory), and keeps programs from accessing each other's memory (security). 
~ Each 'Virtual Address' has its own 32-bt address space.
~ Virtual Memory fills gaps/holes in RAM for address space efficiency.
~ The tradeoff for virtual memory is that we have to 'translate' every single memory access from 'VA' to 'PA'.


TLB translation and Cache lookup at the same time:
  1) Use 'virtual page' bits to index the 'TLB'
  2) Use 'page offset bits' to index the 'cache'
  3) TLB -> Physical Page
  4) Cache -> Physical Tag





































** END ** 
