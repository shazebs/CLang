/*
Author: Shazeb Suhail
Date: Thu.Feb.29.2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <stdbool.h>

#define VIRTUAL_MEMORY_SIZE 0x000FFFFF // 1M bytes
#define PHYSICAL_MEMORY_SIZE 0x7FFFF // 512K bytes

// Function to display a number as binary with LSB to the right.
void printBinary(int number) {
    int index = 0;
    for (int i = 15; i >= 0; i--)
    {
        int bit = (number >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

int getRandomNumber(int max) {
  return rand() % max;
}

void pressEnter() {
  printf("\nEnter 1 to continue...");
  int x;
  scanf("%d", &x);
}

void formatSpacing(int i) {
  if (i < 10)
    printf("  ");
  else if (i < 100)
    printf(" ");
}

int main() {
    // Seed random number generator with the current time
    srand((unsigned int) time(NULL));

    unsigned int pageSize, virtualAddress, physicalAddress;
    int pageSizeOption;

    // Prompt for page size
    printf("Enter page size (1 for 4K, 2 for 8K): ");
    scanf("%d", &pageSizeOption);

    if (pageSizeOption == 1)
      pageSize = 4 * 1024;
    else if (pageSizeOption == 2)
      pageSize = 8 * 1024;
    else {
        printf("Invalid page size.\n");
        return 1;
    }

    // Display page size in decimal, binary and hex
    printf("\nPage size (decimal): %d\n", pageSize-1);
    printf("Page size (binary): ");
    printBinary(pageSize-1);
    printf("Page size (hex): 0x%08X\n", (pageSize-1));

    // Depending on user input, calculate how many indices can exist in virtual page table and page frames table
    int numPages = (VIRTUAL_MEMORY_SIZE + 1) / pageSize;
    int numFrames = (PHYSICAL_MEMORY_SIZE + 1) / pageSize;
    printf("\nVirtual address space = %d virtual pages\n", numPages);
    printf("Physical address space = %d page frames\n", numFrames);

    pressEnter();
    printf("\nVIRTUAL ADDRESS SPACE: virtual pages\n");

    // Create virtual page table
    uint32_t pageTable[numPages];
    for (int i = 0; i < numPages; i++)
    {
      uint32_t pageEntry = i * pageSize;
      pageTable[i] = pageEntry;

      // display address range for current page table index
      formatSpacing(i);
      printf("[%d] = %7d --> %d\n", i, pageTable[i], (pageTable[i]+pageSize-1));
    }

    pressEnter();
    printf("PHYSICAL ADDRESS SPACE: page frames\n");

    // Create page frames table
    int frameTable[numFrames];
    for (int i = 0; i < numFrames; i++)
    {
      frameTable[i] = -1;
      bool duplicateEntry = true;

      // Keep generating random virtual page indices until no duplicate entry is found
      while (duplicateEntry)
      {
        int randomVirtualPageIndex = getRandomNumber(numPages);
        bool indexMatch = false;

        // Check if randomly selected virtual page is already mapped
        for (int j = 0; j <= i; j++)
        {
          if (frameTable[j] == randomVirtualPageIndex)
            indexMatch = true;
        }

        // Insert the non-duplicate randomly selected virtual page index into page frames table
        if (indexMatch == false)
        {
          frameTable[i] = randomVirtualPageIndex;
          duplicateEntry = false;
        }
      }

      // Display page frame's mapped virtual page index for the physical address space
      formatSpacing(i);
      printf("Frame[%d]: %3d | %7d --> %d\n", i, frameTable[i], (i*pageSize), ((i*pageSize) + pageSize-1));
    }

    // Prompt for virtual memory addresses
    for (int i = 0; i < 10; i++)
    {
      physicalAddress = -1;
      int vpIndex, pfIndex, offset;

      printf("\nEnter a virtual memory address in Hexadecimal (between 0x00000000 and 0x0007FFFF): 0x");
      scanf("%8X", &virtualAddress);
      printf("You entered virtual address 0x%08X (hex) which equals to %u (decimal)\n", virtualAddress, virtualAddress);

      // exit program clause
      if (virtualAddress == -1) break;

      if (virtualAddress > VIRTUAL_MEMORY_SIZE) {
          printf("Virtual address out of range.\n");
          continue;
      }

      // Find virtual address on virtual page table
      for (int vp = 0; vp < numPages; vp++)
      {
        if (virtualAddress >= pageTable[vp] && virtualAddress <= (pageTable[vp]+pageSize-1))
        {
          printf("(Virtual Address exists on Virtual Page: %d)\n", vp);
          vpIndex = vp;
          offset = virtualAddress - pageTable[vp];
          break;
        }
      }
      // Find virtual page index on page frames table
      for (int pf = 0; pf < numFrames; pf++)
      {
        if (frameTable[pf] == vpIndex)
        {
          printf("(Virtual page is mapped to Page Frame: %d)\n", pf);
          pfIndex = pf;

          // Translate virtual address to physical address
          physicalAddress = (pfIndex * pageSize + offset);
          break;
        }
      }

      // Display physical address
      if (physicalAddress != -1) {
        printf("\nThis virtual address translates to physical address: 0x%08x (hex) | %d (dec)\n", physicalAddress, physicalAddress);
      }
      else {
        printf("\nCurrently on Disk.\n");
      }
    }

    // End program
    return 0;
}
