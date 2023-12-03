#include <stdio.h>
#include <stdlib.h>

#define VIRTUAL_MEMORY_SIZE 0x100000 // 1M bytes
#define PHYSICAL_MEMORY_SIZE 0x80000 // 512K bytes

// Function to display a number as binary
void printBinary(int number) {
    for (int i = 15; i >= 0; i--) {
        int bit = (number >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");
}

// Function to map virtual address to physical address
unsigned int mapVirtualToPhysical(unsigned int virtualAddress, unsigned int pageSize, unsigned int *pageTable, int tableSize) {
    unsigned int pageNumber = virtualAddress / pageSize;
    unsigned int offset = virtualAddress % pageSize;

    if (pageNumber < tableSize) {
        return (pageTable[pageNumber] * pageSize) + offset;
    }
    return -1; // Address not in physical memory
}

int main() {
    unsigned int pageSize, virtualAddress, physicalAddress;
    int pageSizeOption;

    // Prompt for page size
    printf("Enter page size (1 for 4K, 2 for 8K): ");
    scanf("%d", &pageSizeOption);

    if (pageSizeOption == 1) {
        pageSize = 4095; // 4K
    } else if (pageSizeOption == 2) {
        pageSize = 8191; // 8K
    } else {
        printf("Invalid page size.\n");
        return 1;
    }

    // Display page size in binary and hex
    printf("Page size (binary): ");
    printBinary(pageSize);
    printf("Page size (hex): 0x%X\n", pageSize);

    // Page table
    int tableSize = PHYSICAL_MEMORY_SIZE / pageSize;
    unsigned int pageTable[tableSize];
    for (int i = 0; i < tableSize; i++) {
        pageTable[i] = i;
    }

    // Prompt for virtual memory address
    printf("Enter virtual memory address (hex): ");
    scanf("%x", &virtualAddress);

    if (virtualAddress >= VIRTUAL_MEMORY_SIZE) {
        printf("Virtual address out of range.\n");
        return 1;
    }

    printf("Input virtual memory address: 0x%X\n", virtualAddress);

    // Map to physical address
    physicalAddress = mapVirtualToPhysical(virtualAddress, pageSize, pageTable, tableSize);
    
    if (physicalAddress != -1 && physicalAddress < PHYSICAL_MEMORY_SIZE) {
        printf("Physical memory address: 0x%X\n", physicalAddress);
    } else {
        printf("Result: Currently on Disk.\n");
    }

    return 0;
}
