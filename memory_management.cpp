#include<stdio.h>
#include <stdbool.h>
#include<stdlib.h>
#define PAGE_COUNT 5
#define CHECKPOINT_INTERVAL 5
#define LEVEL1SIZE 10
#define LEVEL2SIZE 10



int pgSize, memSize;
int nFrames, nFree;
int Frames[100];

struct PageTable {
    int pageNo;
    int frameNo;
    struct PageTable* next;
};

struct Level2table {
    int pageNo;
    int frameNo;
    struct PageTable* pagetable;
    struct Level2table* next;
};

struct Level1table {
    int level1no;
    struct Level2table* level2table[LEVEL2SIZE];
};

struct freeFrame {
    int frameNo;
    struct freeFrame* next;
};

struct freeFrame* fHead;

struct Level1table* Process[10];

void init() {
    // Initialize variables
    pgSize = 4; // Page size in KB
    memSize = 100; // Memory size in KB
    nFrames = memSize / pgSize; // Number of frames in memory
    nFree = nFrames; // All frames are initially free

    // Initialize frames array
    for (int i = 0; i < nFrames; i++) {
        Frames[i] = i;
    }

    // Initialize free frames list
    fHead = (struct freeFrame*)malloc(sizeof(struct freeFrame));
    fHead->frameNo = 0;
    fHead->next = NULL;
    struct freeFrame* temp = fHead;
    for (int i = 1; i < nFrames; i++) {
        temp->next = (struct freeFrame*)malloc(sizeof(struct freeFrame));
        temp = temp->next;
        temp->frameNo = i;
        temp->next = NULL;
    }

    // Initialize process array
    for (int i = 0; i < 10; i++) {
        Process[i] = NULL;
    }
}

void printtable_hierarchial(struct Level2table* head) {
    printf("printtable start\n");
    for (struct Level2table* temp = head; temp != NULL; temp = temp->next)
        printf("\tPage %d: Frame %d\n", temp->pageNo, temp->frameNo);
    printf("printtable end\n");
}

void allocate_hierarchial() {
    int p, m, np;
    struct Level2table* head, *temp, *prev;
    printf("Process ID: ");
    scanf("%d", &p);
    if(Process[p]==NULL){
    printf("Memory required in KB: ");
    scanf("%d", &m);
    np = m / pgSize;
    if (m * 1.0 / pgSize > np) np++;
    printf("No. of pages = %d\n", np);

    if (np > nFree) {
        printf("Not available!\n");
        return;
    }
    int count = 0;
    for (int i = 0; i < LEVEL1SIZE; i++) {
       
        if (Process[p] == NULL) {
            Process[p] = (struct Level1table*)malloc(sizeof(struct Level1table));
            Process[p]->level1no = p;
            for (int j = 0; j < LEVEL2SIZE; j++) {
                Process[p]->level2table[j] = NULL;
            }
          
        }
       
        if (count >= np) break;
        for (int j = 0; j < LEVEL2SIZE; j++) {
            if (Process[p]->level2table[i] == NULL) {
                Process[p]->level2table[i] = (struct Level2table*)malloc(sizeof(struct Level2table));
                Process[p]->level2table[i]->pageNo = i;
                Process[p]->level2table[i]->frameNo = fHead->frameNo;
                Process[p]->level2table[i]->pagetable = NULL;
                struct freeFrame* t = fHead;
                fHead = fHead->next;
                free(t);
                nFree--;
                count++;
                if (count == 1) head = Process[p]->level2table[i];
                else prev->next = Process[p]->level2table[i];
                prev = Process[p]->level2table[i];
                break;
            }
        }
        }
    

    head->pagetable = (struct PageTable*)malloc(sizeof(struct PageTable));
    head->pagetable->pageNo = head->pageNo;
    head->pagetable->frameNo = head->frameNo;
    head->pagetable->next = NULL;
    struct PageTable* pt_prev = head->pagetable;
    int count1 = 1;
    for (struct Level2table* lt = head->next; lt != NULL && count1 < np; lt = lt->next) {
        lt->pagetable = (struct PageTable*)malloc(sizeof(struct PageTable));
        lt->pagetable->pageNo = lt->pageNo;
        lt->pagetable->frameNo = lt->frameNo;
        lt->pagetable->next = NULL;
        pt_prev->next = lt->pagetable;
        pt_prev = lt->pagetable;
        count1++;
    }
            printf("Memory allocated successfully!\n");
    }
    else{
        printf("Process ID already exists");
    }
}

void deallocate_hierarchial() {
    int p, m, np;
    printf("Process ID: ");
    scanf("%d", &p);
    if (Process[p] == NULL) {
        printf("Process %d does not exist!\n", p);
        return;
    }
    printf("Memory to be deallocated in KB: ");
    scanf("%d", &m);
    np = m / pgSize;
    if (m * 1.0 / pgSize > np) np++;
    printf("No. of pages = %d\n", np);
    int count = 0;
    for (int i = 0; i < LEVEL1SIZE; i++) {
        if (count >= np) break;
        if (Process[p]->level2table[i] != NULL) {
            int f = Process[p]->level2table[i]->pagetable->frameNo;
            printf("frameno %d %d\n", f, i);
            struct freeFrame* t = (struct freeFrame*)malloc(sizeof(struct freeFrame));
            t->frameNo = f;
            t->next = fHead;
            fHead = t;
            nFree++;
            struct PageTable* tpt = Process[p]->level2table[i]->pagetable;
            Process[p]->level2table[i]->pagetable = Process[p]->level2table[i]->pagetable->next;
            free(tpt);
            struct Level2table* tlt = Process[p]->level2table[i];
            Process[p]->level2table[i] = NULL;
            free(tlt);
            count++;
        }
    }
    Process[p]=NULL;

    if (count == np) {
        printf("Memory deallocated successfully!\n");
    } else {
        printf("Not enough memory allocated to process %d!\n", p);
    }
}

void showtables_hierarchial() {
    int k=0;
    for (int i = 0; i < 10; i++) {
        if (Process[i] != NULL) {
            printf("Level 1 Table %d\n", Process[i]->level1no);
            for (int j = 0; j < LEVEL2SIZE; j++) {
                if (Process[i]->level2table[j] != NULL) {
                    k=1;
                    printf("\tLevel 2 Table %d\n", j);
                    printf("\tPage %d: Frame %d\n", Process[i]->level2table[j]->pageNo, Process[i]->level2table[j]->frameNo);
                }
            }
        }
        
    }
    if(!k){
        printf("Not Process allocated yet!\n");
    }
}

void addressmap_hierarchial() {
    int p, l, pgn, offset, pa;
    printf("Process ID: ");
    scanf("%d", &p);
    
    if(Process[p]!=NULL){
        printf("Logical address: ");
    scanf("%d", &l);
    pgn = l / pgSize;
    offset = l % (pgSize*1024);
    if (pgn >= LEVEL2SIZE) {
        printf("Invalid logical address!\n");
        return;
    }
    
    struct Level2table* lt = Process[p]->level2table[pgn];
    if (lt == NULL || lt->pagetable == NULL) {
        printf("Page fault!\n");
        return;
    }
    pa = lt->pagetable->frameNo * pgSize*1024 + offset;
    printf("Physical address: %d\n", pa);
}
else {
    printf("Not allocated\n");
    return;
}
}

void show_freeframes_hierarchial() {
    printf("Free Frames: ");
    if (fHead == NULL) {
        printf("None\n");
        return;
    }
    struct freeFrame* temp = fHead;
    while (temp != NULL) {
        printf("%d ", temp->frameNo);
        temp = temp->next;
    }
    printf("\n");
}
typedef struct {
    int page_number;
    bool dirty_bit;
} Page;

void checkpoint(Page pages[], int checkpoint_start, int checkpoint_end) {
    printf("Checkpointing Pages %d to %d...\n", checkpoint_start, checkpoint_end);
    for (int i = checkpoint_start; i <= checkpoint_end; i++) {
        pages[i].dirty_bit = false;
    }
}
int pgsize, memsize;
int nframes, nfree;
int frames[100];
typedef struct pagetable {
	int pageno;
	int frameno;
	struct pagetable* next;
}Pagetable;
Pagetable* process[10];
typedef struct freeframe {
	int frameno;
	struct freeframe* next;
}Freeframe;
Freeframe* fhead;
void printtable(Pagetable *head)
{
  for(Pagetable *temp = head; temp != NULL; temp = temp->next)
    printf("Page %d: Frame %d\n", temp->pageno, temp->frameno);
}
void allocate()
{
	int p, m, np;
	Pagetable *head, *temp, *prev;
	printf("Process ID: ");
	scanf("%d", &p);
	printf("Memory required in KB: ");
	scanf("%d", &m);
	np = m / pgsize;
	if(m*1.0/pgsize > np) np++; 
	printf("No. of pages = %d\n", np);
	if(np > nfree) {
		printf("Not available!\n");
		return;
	}
	for(int i = 0; i < np; i++)
	{
		temp = (Pagetable*)malloc(sizeof(Pagetable));		
		temp->pageno = i;
		temp->frameno = fhead->frameno;
		temp->next = NULL;
		Freeframe* t = fhead;
		fhead = fhead->next;
		free(t);
		nfree--;
		if(i == 0) head = temp;
		else prev->next = temp;
		prev = temp;
	}
	process[p] = head;
	printtable(process[p]);
}
void deallocate()
{
	int p;
	printf("PID: ");
	scanf("%d", &p);
    //pd:Pointer to a Pagetable node used for traversing the allocated memory page list.
    //t:Pointer to a Pagetable node used for iterating through the allocated memory pages.
    //d: Pointer to a Pagetable node used for storing the current Pagetable node to be deallocated.
    //f: Variable to store the frame number of the current Pagetable node to be deallocated.
//pf: Pointer to a Freeframe node used for traversing the free frame list.
//tf: Pointer to a Freeframe node used for creating and linking new free frame nodes.
	Pagetable *pd, *t, *d;
	int f;
	Freeframe *pf, *tf;
    pf = fhead;
	if(fhead != NULL) for(; pf->next != NULL; pf = pf->next);
	pd = process[p];
	if(process[p] == NULL) {
		printf("Not allocated!\n");
		return;
	}
	for(t = pd; t != NULL;)
	{
		d = t;
		f = d->frameno;
		tf = (Freeframe*)malloc(sizeof(Freeframe));
		tf->frameno = f;
		tf->next = NULL;
        if(pf != NULL) {
            pf->next = tf;
            pf = tf;
        }
        else {
            pf = tf;
            fhead = pf;
        }
		t = t->next;
        nfree++;
		free(d);
	}
	process[p] = NULL;
	printf("Deallocated!\n");
}
void showtables()
{
    int k=0;
	for(int i = 0; i < 10; i++) {
        if(process[i] != NULL) { k=1; printf("PID: %d\n", i);}
        printtable(process[i]);
		if(process[i] != NULL) printf("\n");
		
	}
	if(!k){
	    printf("Processes are not yet allocated\n");
	}
}
void addressmap()
{
	int p, la, pn, fn, o, pa;
	printf("PID: ");
	scanf("%d", &p);
	
	
	Pagetable *h = process[p], *t;
	t = h;
	for(int i = 0; i < pn; i++)
	{
		t = t->next;
		
	}
	if(t==NULL){
	    printf("Not allocated!\n");
	    return;
	}
	else {
	    printf("Logical address: ");
	scanf("%d", &la);
	pn = la / (pgsize * 1024);
	o = la % (pgsize * 1024);
	fn = t->frameno;
	pa = fn * pgsize * 1024 + o;
	printf("Physical address: %d\n", pa);
	}
	
	
}
void showfree()
{
  for(Freeframe *temp = fhead; temp != NULL; temp = temp->next)
    printf("%d ", temp->frameno);
  printf("\n");
}
int dirtybit_checkpointing(){
    Page pages[PAGE_COUNT];
    int i, page_faults = 0, checkpoint_counter = 0;

    // Initialize pages
    for (i = 0; i < PAGE_COUNT; i++) {
        pages[i].page_number = -1;
        pages[i].dirty_bit = false;
    }

    // Simulated page references
    //int page_references[] = {1, 2, 3, 4, 5, 1, 2, 6, 7, 8, 1, 3, 7, 9, 2, 4, 6, 8, 9, 10};
    int n;
    printf("Enter the number of page references:");
    scanf("%d",&n);
    int page_references[n];
    printf("Enter the page references:");
    for(i=0;i<n;i++){
        scanf("%d",&page_references[i]);
    }

    // Simulate page replacement algorithm with dirty bit and checkpointing
    for (i = 0; i < sizeof(page_references) / sizeof(page_references[0]); i++) {
        int page_number = page_references[i];
        bool page_found = false;

        // Check if page is already in memory
        for (int j = 0; j < PAGE_COUNT; j++) {
            if (pages[j].page_number == page_number) {
                page_found = true;
                pages[j].dirty_bit = true; // Set dirty bit as page is modified
                break;
            }
        }

        if (!page_found) {
            // Find a page to replace using FIFO
            int replace_index = page_faults % PAGE_COUNT;

            // Checkpoint interval reached, perform checkpointing
            if (checkpoint_counter == CHECKPOINT_INTERVAL) {
                checkpoint(pages, replace_index, PAGE_COUNT - 1);
                checkpoint_counter = 0;
            }

            // Replace page
            pages[replace_index].page_number = page_number;
            pages[replace_index].dirty_bit = true; // Set dirty bit for the newly loaded page
            page_faults++;
            checkpoint_counter++;

            // Print the current state of pages after each page reference
            printf("Page %d (Page Fault): ", page_number);
            for (int j = 0; j < PAGE_COUNT; j++) {
                if (pages[j].page_number == -1) {
                    printf("-");
                } else {
                    printf("%d", pages[j].page_number);
                    if (pages[j].dirty_bit) {
                        printf("*");
                    }
                }
                printf(" ");
            }
            printf("\n");
        } else {
            // Print the current state of pages after each page reference
            printf("Page %d: ", page_number);
            for (int j = 0; j < PAGE_COUNT; j++) {
                if (pages[j].page_number == -1) {
                    printf("-");
                } else {
                    printf("%d", pages[j].page_number);
                    if (pages[j].dirty_bit) {
                        printf("*");
                    }
                }
                printf(" ");
            }
            printf("\n");
        }
    }

    printf("\nTotal Page Faults: %d\n", page_faults);
    
    
}
int hierarchial_pagetable() {
    int choice;
    init();
       while(choice!=6) {
        printf("1. Allocate memory\n");
        printf("2. Deallocate memory\n");
        printf("3. Show page tables\n");
        printf("4. Show address map\n");
        printf("5. Show free frames\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                allocate_hierarchial();
                break;
            case 2:
                deallocate_hierarchial();
                break;
            case 3:
                showtables_hierarchial();
                break;
            case 4:
                addressmap_hierarchial();
                break;
            case 5:
                show_freeframes_hierarchial();
                break;
            case 6:
                break;
            default:
                printf("Invalid choice!\n");
                break;
        }
       }
    
    return 0;
}

int main()
{   int choose;
    
    
    while(1){
        printf("\n1.Simple Page table Paging  2.Dirtybit and checkpointing  3.Hierarchial page table 4.Exit\nChoose an option:");
        scanf("%d",&choose);
    switch(choose){
        case 1: {
            Freeframe *temp, *prev;	
	printf("Total physical memory in KB: ");
	scanf("%d", &memsize);
	printf("Page size in KB: ");
	scanf("%d", &pgsize);
	nframes = memsize / pgsize;
	printf("No. of frames = %d\n", nframes);
	for(int i = 0; i < nframes/2; i++) {
		int fr = rand()%nframes;
        if(frames[fr] != 1) {
            frames[fr] = 1;
            nfree++;
            temp = (Freeframe*)malloc(sizeof(Freeframe));
            temp->frameno = fr;
            temp->next = NULL;
            if(nfree == 1) fhead = temp;
            else prev->next = temp;
            prev = temp;
        }
        else i--;
	}
	int ch = 0;
	do {
		printf("\n1. Process Request 2. Deallocate 3. Show all Page Tables 4. Show all Free Frames 5. Address Mapping 6. Exit\nChoice: ");
		scanf("%d", &ch);
		switch(ch) {
			case 1: allocate(); break;
			case 2: deallocate(); break;
			case 3: showtables(); break;
			case 4: showfree(); break;
			case 5: addressmap(); break;
			case 6: break;
		}

	}while(ch!=6);
	break;
        }
        case 2: dirtybit_checkpointing(); break;
        case 3: hierarchial_pagetable(); break;
        case 4: exit(0);
    }
    }
    
	
    
	return 0;
}
