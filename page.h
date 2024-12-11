#ifndef PAGE_H
#define PAGE_H

// In here I will use the a structure for tracking physical pages in memory
struct ppage {
  struct ppage *next;           // Pointer to the next physical page
  struct ppage *prev;           // Pointer to the previous physical page
  void *physical_addr;          // Pointer to the physical address of the page
};

void init_pfa_list(void);                                
struct ppage *allocate_physical_pages(unsigned int npages); 
void free_physical_pages(struct ppage *ppage_list);      

#endif 
