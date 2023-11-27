** Virtual Memory Key Terms **
----------------------------------------------------------
Page Table - 

Page Fault - 

Page Table Cache - 

Translation Lookup(Look-aside) Buffer (TLB) - 


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
  1) You can make TLB pages bigger

~ In Virtual Memory, 'Virtual Addresses' are mapped to 'Physical Address' in RAM.
~ A 'Virtual Address' consists of a 'Virtual Page Number' and 'Page Offset'. 
~ A 'Page Table' must remain in 'Main Memory' at all times, otherwise we won't be able to access anything.
~ If a 'Page Table Entry' does not exist in the 'TLB', it is loaded from 'Memory' into the 'TLB'. 
~ Each program needs its own 'Page Table'. 
~ With a 'Virtual Cache', TLB lookups only occurs when we miss in cache.
~ Two programs cannot share a 'Virtual Cache'. 
