//Includes.
#include<stdio.h>
#include<string.h>

//Structures to represent the disk layout.
typedef struct inode
{
    char name[8];
    int size;
    int block_pointers[8];
    int used;
}inode;

typedef struct super_block
{
    char free_block_list[127];
    inode x[16]; 
}super_block;

typedef struct disk_layout
{
    super_block sb;
    char data_block[127][1024];
}disk_layout;
disk_layout dl;

//Variables to count number of files live, number of blocks live.
int number_of_files_live,number_of_blocks_live;

//Functions.
void create(char [],int);
void delete(char []);
void read(char [],int,char []);
void write(char [],int,char []);
void ls();
int get_first_free_file_index(); //It is needed inside the create function. Scan all the 16 inodes and return the index of the first inode whose used=0.   
int get_first_free_block_index(); //It is needed inside the create function. Scan all the 127 blocks and return the index of the first free block.
int get_index_by_name(char []); //It is needed inside the create,delete,read,write function to get the index number(o to 15) of the file by the file name. 

//The driver funcion.
int main()
{
    //Initializing the structures.
    for(int i=0;i<127;i++)
        dl.sb.free_block_list[i]=0;
    for(int i=0;i<16;i++)
        dl.sb.x[i].used=0;

    char k;
    while(1)
    {
        printf("===================================================================\n");
        printf("a. Create a file.\n");
        printf("b. Delete a file.\n");
        printf("c. Read from a block of a file.\n");
        printf("d. Write onto a block of a file.\n");
        printf("e. List the names of all files in the file system and their sizes.\n");
        printf("f. Exit from the program.\n");
        printf("===================================================================\n");
        printf("Enter your choice: ");
        scanf("%c",&k);
        
        //Clearing the input buffer. If it is not done, unused inputs from the input buffer will be read in the next scan if type matches.
        int c;
        while((c=getchar())!='\n'&&c!=EOF);
        
        switch(k)
        {
            case 'a':
            {
                //Take input.
                char name[8];
                printf("Enter the File name(upto 5 characters altogether): ");
                fgets(name,8,stdin);
                
                int size;
                printf("Enter the size of the file in terms of number of blocks(1 to 8): ");
                scanf("%d",&size);

                //Clearing the input buffer. It is needed. Otherwise, the "Enter" key will be read when a character or string will be read next time.
                int c;
                while((c=getchar())!='\n'&&c!=EOF);

                //Call the function.
                create(name,size);

                break;
            }
            case 'b':
            {
                //Take input.
                char name[8];
                printf("Enter the File name(upto 5 characters altogether): ");
                fgets(name,8,stdin);

                //Call the function.
                delete(name);

                break;
            }
            case 'c':
            {
                //Take input.
                char name[8];
                printf("Enter the File name(upto 5 characters altogether): ");
                fgets(name,8,stdin); //fgets(buffer,n,stdin) can take upto n-3 characters successfully.
                
                int block_number;
                printf("Enter the block number(0 to (size of the file-1)): ");
                scanf("%d",&block_number);

                //Clearing the input buffer. If it is not done, unused inputs from the input buffer will be read in the next scan if type matches.
                int c;
                while((c=getchar())!='\n'&&c!=EOF);
                
                char buffer[1024];

                //Call the function.
                read(name,block_number,buffer);
                
                break;
            }  
            case 'd':
            {
                //Take input.
                char name[8];
                printf("Enter the File name(upto 5 characters altogether): ");
                fgets(name,8,stdin);
                
                int block_number;
                printf("Enter the block number(0 to (size of the file-1)): ");
                scanf("%d",&block_number);
                
                //Clearing the input buffer. If it is not done, unused inputs from the input buffer will be read in the next scan if type matches.
                int c;
                while((c=getchar())!='\n'&&c!=EOF);
                
                char buffer[1024]; 
                
                //Call the function.
                write(name,block_number,buffer);

                break;
            }
            case 'e':
            {
                ls();
                break;
            }
            case 'f':
                return 0;
            default:
                printf("[ERROR]: Wrong choice. Try again.\n");
        }
    }
}

void create(char name[],int size)
{
    //Verify input.
    int first_free_file_index=get_first_free_file_index();
    if(first_free_file_index==-1)
    {
        printf("[ERROR]: 16 files already exists. New file can not be created before deleting any existing file.\n");
        return;
    }

    int index=get_index_by_name(name);
    if(index!=-1)
    {
        printf("[ERROR]: Another file with this name already exists.\n");
        return;
    }

    if(strlen(name)-1>5)
    {
        printf("[ERROR]: File name must be upto 5 characters altogether.\n");
        return;
    }

    if(size<1 || size>8)
    {
        printf("[ERROR]: Size must be from 1 to 8.\n");
        return;
    }

    if(size==8 && number_of_files_live==15 && number_of_blocks_live==120) //Only trigers when currently there are 15 files live and each of them has 8 blocks and the 16th file is trying to acquire 8 blocks.
    {
        printf("[ERROR]: Only 7 blocks are available. Because existing 15 files has acquired 8 blocks each. Total number of blocks=15*8+7\n");
        return;
    }

    //Actual task.
    strcpy(dl.sb.x[first_free_file_index].name,name);
    dl.sb.x[first_free_file_index].size=size;
    dl.sb.x[first_free_file_index].used=1;
    number_of_files_live++;
    
    int first_free_block_index;
    for(int i=0;i<size;i++)
    {
        first_free_block_index=get_first_free_block_index();
        dl.sb.free_block_list[first_free_block_index]=1;
        dl.sb.x[first_free_file_index].block_pointers[i]=first_free_block_index;
        number_of_blocks_live++;
    }

    printf("File created successfully.\n");
}

void delete(char name[])
{
    //Verify input.
    int index=get_index_by_name(name);
    if(index==-1)
    {
        printf("[ERROR]: File with this name does not exist.\n");
        return;
    }

    //Actual task.
    dl.sb.x[index].used=0;
    number_of_files_live--;
    for(int i=0;i<dl.sb.x[index].size;i++)
    {
        dl.sb.free_block_list[dl.sb.x[index].block_pointers[i]]=0;
        dl.data_block[dl.sb.x[index].block_pointers[i]][0]=0;
        number_of_blocks_live--;
    }

    printf("File deleted successfully.\n");
}

void read(char name[],int block_number,char buffer[])
{
    //Verify input.
    int index=get_index_by_name(name);
    if(index==-1)
    {
        printf("[ERROR]: File with this name does not exist.\n");
        return;
    }

    if(block_number<0 || block_number>dl.sb.x[index].size-1)
    {
        printf("[ERROR]: The size of the file is %d. Hence, Block number must be from 0 to %d.\n",dl.sb.x[index].size,dl.sb.x[index].size-1);
        return;
    }

    //Actual task.   
    strcpy(buffer,dl.data_block[dl.sb.x[index].block_pointers[block_number]]);
    
    printf("Block number %d of the file is read successfully. The content of the block is: ",block_number);
    puts(buffer);
}

void write(char name[],int block_number,char buffer[])
{
    //Verify input.
    int index=get_index_by_name(name);
    if(index==-1)
    {
        printf("[ERROR]: File with this name does not exist.\n");
        return;
    }

    if(block_number<0 || block_number>dl.sb.x[index].size-1)
    {
        printf("[ERROR]: The size of the file is %d. Hence, Block number must be from 0 to %d.\n",dl.sb.x[index].size,dl.sb.x[index].size-1);
        return;
    }

    //Actual task.
    printf("Write into the block(upto 1021 characters altogether): ");
    fgets(buffer,1024,stdin); //fgets(buffer,n,stdin) can take upto n-3 characters successfully.
    if(strlen(buffer)-1>1021)
    {
        printf("Number of characters must be upto 1021 characters altogether");
        return;
    }

    strcpy(dl.data_block[dl.sb.x[index].block_pointers[block_number]],buffer); 

    printf("Block number %d of the file is written successfully.\n",block_number);  
}

void ls()
{
    printf("---------------------\n");
    printf("File name\tSize\n");
    printf("---------------------\n");
    for(int i=0;i<16;i++)
    {
        if(dl.sb.x[i].used==1)
        {
            printf("%s\b\r\t\t %d\n",dl.sb.x[i].name,dl.sb.x[i].size);
            printf("---------------------\n");
        }
    }
}

int get_first_free_file_index()
{
    for(int i=0;i<16;i++)
        if(dl.sb.x[i].used==0)
            return i;
    return -1;
}

int get_first_free_block_index()
{
    for(int i=0;i<127;i++)
        if(dl.sb.free_block_list[i]==0)
            return i;
}

int get_index_by_name(char name[])
{
    for(int i=0;i<16;i++)
        if(dl.sb.x[i].used==1 && !strcmp(dl.sb.x[i].name,name))
            return i;
    return -1;
}
