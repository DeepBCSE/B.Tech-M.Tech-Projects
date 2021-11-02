#include<stdio.h>
#include<stdlib.h>

typedef struct slot
{
    int data;
    int reference_bit;
    struct slot *next;
}slot;

slot *createCLL(int); //Creates a Circular Linked List.
slot *createslot(); //Creates one slot and initializes the slot data to -1 and the reference bit to 0.
void clock_algo(int [],int,slot *,int); //Does the main job.
slot *search_slot(int,slot []); //Needed inside the clock_algo function to search a box inside each slots. Only then we can say it is a page fault or hit.

int main()
{
    //Take input.

    //Take the boxes.
    printf("==================================================================================\n");
    printf("Enter the number of boxes: ");
    int n;
    scanf("%d",&n);
    if(n<=0)
    {
        printf("[ERROR]: Number of boxes should be positive.");
        exit(0);
    }
    printf("Enter the box identification numbers seperated by space.\n");
    printf("***May be repeted.\n");
    printf("***Enter only %d numbers. Otherwise, only first %d numbers will be considered.\n",n,n);
    printf("***Enter only non-negetive numbers. Because indices can not be negetive.\n");
    int *box=(int *)malloc(sizeof(int)*n);
    for(int i=0;i<n;i++)
    {
        scanf("%d",&box[i]);
        if(box[i]<0)
        {
            printf("[ERROR]: Identification numbers must be non-negetive.");
            exit(0);
        }
    }
    
    //Clearing the input buffer. If it is not done, unused inputs from the input buffer will be read in the next scan if type matches.
    int c;
    while((c=getchar())!='\n'&&c!=EOF);
    
    //Take the number of frames.
    printf("Enter the number of frames: ");
    int m;
    scanf("%d",&m);
    if(m<=0 || m>=n)
    {
        printf("[ERROR]: Number of slots must be positive and less than number of boxes.");
        exit(0);
    }
    printf("==================================================================================\n");

    //Output.
    printf("----------------------------------------------------------------------------------\n");

    //Original output.
    printf("ORIGINAL OUTPUT WITH NUMBER OF SLOTS = %d:\n\n",m);
    slot *head=createCLL(m);
    clock_algo(box,n,head,m);
    free(head);
    printf("----------------------------------------------------------------------------------\n");
   
    //Output with m=m-1.
    if(m-1!=0)
    {
        printf("OUTPUT WITH NUMBER OF SLOTS = %d:\n\n",m-1);
        head=createCLL(m-1);
        clock_algo(box,n,head,m-1);
        free(head);
        printf("----------------------------------------------------------------------------------\n");
    }

    //Output with m=m-1.
    printf("OUTPUT WITH NUMBER OF SLOTS = %d:\n\n",m+1);
    head=createCLL(m+1);
    clock_algo(box,n,head,m+1);
    free(head);
    printf("----------------------------------------------------------------------------------\n");
    printf("==================================================================================\n");
}

slot *createCLL(int m)
{
    slot *head=NULL,*prev=NULL,*curr=NULL;
    for(int i=0;i<m;i++)
    {
       
        prev=curr;
        curr=createslot();
        if(head==NULL)
            head=curr;
        else
            prev->next=curr;
        curr->next=head;  
    }
    return head;
}

slot *createslot()
{
    slot *temp=(slot *)malloc(sizeof(slot));
    if(temp==NULL)
    {
        printf("Memory full!");
        exit(1);
    }
    temp->data=-1;
    temp->reference_bit=0;
    temp->next=NULL;
    return temp;
}

void clock_algo(int box[],int n,slot *head,int m)
{
    printf("List of blocks which got replaced: [");

    int number_of_replacements=0,number_of_page_faults=0,number_of_free_slots=m;
    slot *hand=head;
    for(int i=0;i<n;i++) //Do for each box.
    {
        slot *p=search_slot(box[i],head);
        if(p!=NULL) //Page hit.
        {
            p->reference_bit=1;
            printf("- ");
        } 
        else //Page fault.
        {
            
            if(number_of_free_slots!=0) //Some slots are available. Bring the box in the first empty slot and advance hand.
            {
                
                p=head;
                while(p->data!=-1)
                    p=p->next;
                number_of_free_slots--;
                printf("- ");
            }
            else //No slots are available. Replace the page who arrived earliest as well as reference bit is 0. hand is pointing to the earliest arrived node.
            {
                p=hand;
                while(p->reference_bit==1)
                {
                    p->reference_bit=0;
                    p=p->next;
                }
                printf("%d ",p->data);
                number_of_replacements++;
                hand=p;
                hand=hand->next;
            }
            p->data=box[i];
            number_of_page_faults++;
        }   
    }
        printf("\b]\nTotal number of blocks replaced: %d\n",number_of_replacements);
        printf("Total number of page faults: %d\n",number_of_page_faults);
}

slot *search_slot(int x,slot *head)
{
    slot *p=head;
    while(p->next!=head)
    {
        if(x==p->data)
            return p;
            p=p->next;
    }
    if(x==p->data)
        return p;
    return NULL;
}