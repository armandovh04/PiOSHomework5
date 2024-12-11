#include "page.h"

struct ppage physical_page_array[128];

struct ppage *free_physical_pages = NULL;

void init_pfa_list(void) {
  for (int i = 0; i < 128; i++) {
      physical_page_array[i].physical_addr = (void *)(i * 0x200000);  // 2MB per page, so page i will be at 2MB * i

      if (i < 127) {
          physical_page_array[i].next = &physical_page_array[i + 1];
      } else {
          physical_page_array[i].next = NULL;  // Last page points to NULL
      }

      if (i > 0) {
          physical_page_array[i].prev = &physical_page_array[i - 1];
       } else {
           physical_page_array[i].prev = NULL;  // First page has no previous
       }
   }

   free_physical_pages = &physical_page_array[0];
}

struct ppage *allocate_physical_pages(unsigned int npages) {
  if (npages == 0) {
       return NULL;
   }

   struct ppage *allocated_pages = NULL;

   for (unsigned int i = 0; i < npages; i++) {
       if (!free_physical_pages) {
           return NULL;  // Not enough pages available
       }

      // Unlink the first page from the free list
      struct ppage *page = free_physical_pages;
      free_physical_pages = free_physical_pages->next;

       // If there are still pages in the free list, update their prev pointer to NULL
      if (free_physical_pages) {
          free_physical_pages->prev = NULL;
       }

      page->next = allocated_pages;
      if (allocated_pages) {
           allocated_pages->prev = page;
       }
      allocated_pages = page; 
   }

  return allocated_pages;  // Return the new allocated list
}

// Free the allocated physical pages and add them back to the free list
void free_physical_pages(struct ppage *ppage_list) {
  struct ppage *current = ppage_list;

  while (current) {
      struct ppage *next_page = current->next; 

       current->next = free_physical_pages;
       if (free_physical_pages) {
           free_physical_pages->prev = current;
       }
       free_physical_pages = current; 
       current->prev = NULL;  

       current = next_page;   
  }
}
