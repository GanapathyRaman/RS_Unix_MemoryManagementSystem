#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<signal.h>
#include<string.h>
#include<pthread.h>

time_t now;
int pool_count=0;
int max_no_of_pools,max_size_of_pools;
int each_chunk_size[10][1024];
char filename[20]="Log.txt";

struct mem_pool
{
        char mem_chunk[1024];
        int available_mem;
        int chunks_count;
        struct mem_pool *next;
        int last;
};

struct mem_pool *start=NULL,*ptr=NULL,*temp=NULL;

void check_log_files()
{
        FILE*cfp;
        FILE*fptr;
        int fno,start,end,fsize,i;
        cfp=fopen("a.txt","r");
        if(cfp==NULL)
        {
                perror("Error in opening Config File: ");
        }
        while(!feof(cfp))
        {
                fscanf(cfp,"%d",&fno);
        }
        fclose(cfp);
        system("rm a.txt");
        sprintf(filename,"Log%d.txt",fno);
        cfp=fopen("a.txt","a+");

    fptr=fopen(filename,"a+");
        if(fptr==NULL)
        {
                perror("Error in Opening in Log File: ");
        }

        start=ftell(fptr);
        fseek(fptr,0,2);
        end=ftell(fptr);

        fsize=end-start;
        fclose(fptr);

        if(fsize>500)
        {
                fno++;
                if(fno>10)
                {
                        fno=1;
                        for(i=1;i<=10;i++)
                        {
                                sprintf(filename,"rm Log%d.txt",i);
                        }
                }
                sprintf(filename,"Log%d.txt",fno);
        }
        else
        {
                sprintf(filename,"Log%d.txt",fno);

        }
        fprintf(cfp,"%d",fno);
        fptr=fopen(filename,"w+");
        fclose(fptr);
        fclose(cfp);
}

void man_pool_alloc()
{

        check_log_files();

        FILE*fp;
  fp=fopen(filename,"a+");
        printf("Enter the Maximum No of Pools to be allocated: ");
        fflush(stdin);
        scanf("%d",&max_no_of_pools);
        fflush(stdin);
        printf("\nEnter the Size of the Pools: ");
        fflush(stdin);
        scanf("%d",&max_size_of_pools);
        fflush(stdin);
        fprintf(fp,"\nInitial Memory Configurations");
        fprintf(fp,"\n-----------------------------\n");
        fprintf(fp,"Manually Memory pools are Configured by the User\n\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n\n",ctime(&now));
        fprintf(fp,"Maximum No of Pools   : %d\n",max_no_of_pools) ;
        fprintf(fp,"Maximum Size of Pools : %d\n",max_size_of_pools);
        printf("\nMaximum No of Pools   : %d\n",max_no_of_pools) ;
        printf("Maximum Size of Pools : %d\n\n",max_size_of_pools);
        fclose(fp);
}

void auto_pool_alloc()
{

        check_log_files();

        FILE*cptr;
        FILE*fp;
        char buff1[50],buff2[50];
        cptr=fopen("Config.txt","r");
        fp=fopen("filename","a+");
        if(cptr==NULL)
        {
                perror("");
                fprintf(fp,"\nInitial Memory Configurations");
                fprintf(fp,"\n-----------------------------\n");
                fprintf(fp,"Error In Opening the Configuration file\n");
                time(&now);
                fprintf(fp,"Date & time is: %s\n\n",ctime(&now));
        }

        while(!feof(cptr))
        {

       fscanf(cptr,"%s %d %s %d",buff1,&max_no_of_pools,buff2,&max_size_of_pools);
        }
        fclose(cptr);
        fprintf(fp,"\nInitial Memory Configurations");
        fprintf(fp,"\n----------------------------\n");
        fprintf(fp,"Automatically Memory pools are Configured\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));
        fprintf(fp,"Maximum No of Pools   : %d\n",max_no_of_pools) ;
        fprintf(fp,"Maximum Size of Pools : %d\n",max_size_of_pools);
        fclose(fp);
        printf("Maximum No of Pools   : %d\n",max_no_of_pools) ;
        printf("Maximum Size of Pools : %d\n\n,",max_size_of_pools);
}

void add_mem_pool()
{

        check_log_files();

        int i;
        FILE*fp;
        fp=fopen(filename,"a+");
        fprintf(fp,"\nMemory Pools Allocation");
        fprintf(fp,"\n-----------------------\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n\n",ctime(&now));
        for(i=pool_count;i<max_no_of_pools;i++)
        {
                ptr=(struct mem_pool *)malloc(sizeof(struct mem_pool));
                ptr->available_mem = max_size_of_pools;
                ptr->chunks_count=0;
                bzero(ptr->mem_chunk,1024);
                fprintf(fp,"Memory Pool of Size [%d] bytes is allocated",ptr->available_mem);
                fprintf(fp,"\nStarting Address of the Pool is [%d]\n",ptr);
                if(ptr==NULL)
                {
                        perror("Error Occured");
                        fprintf(fp,"\nError in Allocating the Memory\n");
                }
                if(start==NULL)
                {
                        start=ptr;

            start->next=NULL;
                        start->last=0;
                        pool_count++;
                }
                else
                {

                        for(temp=start;temp->next!=NULL;temp=temp->next);
                        temp->next=ptr;
                        ptr->next=NULL;
                        ptr->last=0;
                        pool_count++;
                }
        }
        fclose(fp);

        printf("You have %d Pools.. Each with the Size of %d\n\n",pool_count,max_size_of_pools);
}

void dealloc_all_pool()
{

        check_log_files();

        int dflag=0,i=1;
        FILE*fp;
        fp=fopen(filename,"a+");
        fprintf(fp,"\nMemory Pools De-allocation");
        fprintf(fp,"\n---------------------------\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));
        if(start==NULL)
        {
                fprintf(fp,"All Memory Pools are already deallocated.. Nothing is left out\n");
        }
        while(start!=NULL)
        {       temp=start;
                fprintf(fp,"De-allocating Memory Pool [%d] of Size [%d] bytes",i,max_size_of_pools);
                fprintf(fp,"\nStaring Address of the Pool is[%d]\n",temp);
                start=start->next;
                free(temp);
                i++;
                dflag++;
    }
        if(dflag==pool_count)
        {
                printf("\nSuccessfully Deallocated %d pools\n\n",max_no_of_pools);
                fprintf(fp,"\nSuccessfully Deallocated %d pools\n",max_no_of_pools);

        }
        fclose(fp);
}

void *mem_chunk(void *arg)
{

        check_log_files();

        int cflag,csize,i,pool_move_count=1,do_count=1;
        FILE*fp;
        fp=fopen(filename,"a+");
        ptr=start;
        printf("Enter the size of Chunk: ");
        fflush(stdin);
        scanf("%d",&csize);
        fprintf(fp,"\nMemory Chunks Allocation");
        fprintf(fp,"\n------------------------\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));
        fprintf(fp,"Trying to Allocate Chunk of size [%d] bytes\n",csize);
        do
        {
                cflag=0;
                if(csize<=ptr->available_mem)
                {
                        for(i=1;i<=csize;i++)
                        {
                                printf("\nEnter the Character to be stored in Chunk: \n");
                                fflush(stdin);
                                scanf("%s",&ptr->mem_chunk[ptr->last]);
                                fflush(stdin);
                                fflush(stdout);
                                fprintf(fp,"Successfully Stored Character [%c] in Pool No [%d] in the address [%d]\n",ptr->mem_chunk[ptr->last],pool_move_count,&ptr->mem_chunk[ptr->last]);
                                ptr->last++;
                                ptr->available_mem--;

                }
                        fprintf(fp,"\n");
                        cflag++;

                        each_chunk_size[(pool_move_count)-1][ptr->chunks_count]=csize;
                        ptr->chunks_count++;
                }
                else
                {
                        printf("\nSorry the request size of memory is not available in Pool [%d]\n",pool_move_count);
                        pool_move_count++;
                        if(pool_move_count<=max_no_of_pools)
                        {
                                printf("\nSearching for required memory in Pool [%d]\n",pool_move_count);
                        }
                        if(ptr!=NULL)
                        {
                                ptr=ptr->next;
                        }
                }
        do_count++;
        }while((do_count<=max_no_of_pools)&&(cflag==0));
        if(cflag==0)
        {
                printf("\nSorry.. Request Memory was Not available in any pool.. Try Deallocating all pool\n");
                fprintf(fp,"Chunk was not created..\n");
                fprintf(fp,"Reason: No Pool was having required capacity at that time\n");
        }
        fclose(fp);
}

void mem_pool_availability()
{

        check_log_files();

        int i;
        FILE*fp;
        fp=fopen(filename,"a+");
        fprintf(fp,"Memory Available in Pools");
        fprintf(fp,"\n-------------------------\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));

  for(temp=start,i=1;temp!=NULL;temp=temp->next,i++)
        {
                printf("In Pool [%d]: %d bytes are Free\n",i,temp->available_mem);
                fprintf(fp,"In Pool [%d]: %d bytes are Free\n",i,temp->available_mem);
        }
        printf("\n");
        fprintf(fp,"\n\n");
        fclose(fp);
}

void mem_chunk_availability()
{

        check_log_files();

        FILE*fp;
        int i=0,j;
        fp=fopen(filename,"a+");
        fprintf(fp,"Memory Available in Chunk");
        fprintf(fp,"\n-------------------------\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));

        temp=start;
        while(temp!=NULL)
        {
                if(temp->chunks_count!=0)
                {
                        printf("In Pool [%d]\n",(i+1));
                        printf("------------\n\n");
                        fprintf(fp,"In Pool [%d]\n",(i+1));
                        fprintf(fp,"------------\n");
                        for(j=0;j<temp->chunks_count;j++)
                        {
                                printf("Size of Chunk [%d]: %d bytes\n",(j+1),each_chunk_size[i][j]);
                                fprintf(fp,"Size of Chunk [%d]: %d bytes\n",(j+1),each_chunk_size[i][j]);
                        }
                        printf("\n");
                        fprintf(fp,"\n");
                        i++;
                        temp=temp->next;
                }
                else

           {
                        printf("In Pool [%d]\n",(i+1));
                        printf("-------------\n\n");
                        fprintf(fp,"In Pool [%d]\n",(i+1));
                        fprintf(fp,"------------\n");
                        printf("No Chunk is available in this Pool\n");
                        fprintf(fp,"No Chunk is available in this Pool\n");
                        printf("\n");
                        fprintf(fp,"\n");
                        i++;
                        temp=temp->next;
                }
                printf("\n");
                fprintf(fp,"\n");
        }
        printf("\n\n");
        fprintf(fp,"\n");
        fclose(fp);
}

void display_pool_contents()
{

        check_log_files();

        FILE*fp;
        fp=fopen(filename,"a+");
        int pno,i,j,k,l=0,chksize;
        printf("You have %d No of Pools\n",pool_count);
        printf("\n Enter the Pool No to view its contents: ");
        scanf("%d",&pno);
        printf("\n");

        temp=start;

        if(pno>pool_count)
        {
                printf("Sorry No such Pool is Available\n");
        }
        else
        {

                fprintf(fp,"Memory Available in Chunk");

     fprintf(fp,"\n-------------------------\n");

                time(&now);
                fprintf(fp,"Date & time is: %s\n",ctime(&now));

                for(i=1;(i<pno)&&(temp!=NULL);i++)
                {
                        temp=temp->next;
                }
                printf("Contents in Pool [%d]\n",(i));
                printf("---------------------\n");

                fprintf(fp,"Contents in Pool [%d]\n",(i));
                fprintf(fp,"---------------------\n");

                for(j=0;j<temp->chunks_count;j++)
                {
                        chksize=each_chunk_size[i-1][j];
                        printf("In Chunk [%d]: ",(j+1));
                        for(k=0;k<chksize;k++)
                        {
                                printf("%c ",temp->mem_chunk[l]);
                                fprintf(fp,"%c ",temp->mem_chunk[l]);
                                l++;
                                l++;
                        }
                        printf("\n\n");
                        fprintf(fp,"\n");
                }
        }
        fclose(fp);
}

void display_chunk_contents()
{

        check_log_files();

        FILE*fp;
        fp=fopen(filename,"a+");

        int plno,chno,i,j,k,l,sum=0,chksize;


 printf("You have %d No of Pools\n",pool_count);
        printf("\n Enter the Pool No: ");
        scanf("%d",&plno);
        printf("\n");


        if(plno>pool_count)
        {
                printf("Sorry No such Pool is Available\n\n");
        }
        else
        {
                printf("\n Enter the Chunk No to view its contents: ");
                scanf("%d",&chno);
                printf("\n");

                temp=start;
                for(i=1;(i<plno)&&(temp!=NULL);i++)
                {
                        temp=temp->next;
                }

                if(chno>temp->chunks_count)
                {
                        printf("Sorry No such Chunk is Available\n");
                }
                else
                {

                        printf("Contents in Pool [%d]\n",(i));
                        printf("---------------------\n");
                        fprintf(fp,"Contents in Pool [%d]\n",(i));
                        fprintf(fp,"---------------------\n");

                        time(&now);
                        fprintf(fp,"Date & time is: %s\n",ctime(&now));

                        chksize=each_chunk_size[i-1][chno-1];

                        for(j=0;j<(chno-1);j++)
                        {
                                sum=sum+(each_chunk_size[i-1][j]);
                        }

     l=sum;
                        printf("In Chunk [%d]: ",(chno));

                        for(k=0;k<chksize;k++)
                        {
                                printf("%c ",temp->mem_chunk[l]);
                                fprintf(fp,"%c ",temp->mem_chunk[l]);
                                l++;
                        }
                        printf("\n\n");
                        fprintf(fp,"\n");
                }
        }
        fclose(fp);
}



void add_individual_pool()
{

        check_log_files();

        int i,pno;
        FILE*fp;

        fp=fopen(filename,"a+");
        printf("\nYou Have %d No of Pools\n",pool_count);
        printf("Enter the No of Pools to be allocated : ");
        scanf("%d",&pno);
        printf("\n");
        if((pno+pool_count)>max_no_of_pools)
        {
                printf("Sorry you can allocate only maximum of %d pools at this time\n",(max_no_of_pools-pool_count));
        }
        else
        {
                fprintf(fp,"Memory Pools Allocation");
                fprintf(fp,"\n------------------------\n");
                time(&now);
                fprintf(fp,"Date & time is: %s\n",ctime(&now));
                for(i=1;(i<=pno)&&(pool_count <=max_size_of_pools);i++)
                {

   ptr=(struct mem_pool *)malloc(sizeof(struct mem_pool));
                        ptr->available_mem = max_size_of_pools;
                        ptr->chunks_count=0;
                        bzero(ptr->mem_chunk,1024);
                        fprintf(fp,"Memory Pool of Size [%d] bytes is allocated",ptr->available_mem);
                        fprintf(fp,"\nStaring Address of the Pool is [%d]\n",ptr);
                        if(ptr==NULL)
                        {
                                perror("Error Occured");
                                fprintf(fp,"\nError in Allocating the Memory\n");
                        }

                        for(temp=start;temp->next!=NULL;temp=temp->next);
                        temp->next=ptr;
                        ptr->next=NULL;
                        ptr->last=0;
                        pool_count++;
                }
                printf("You have %d No of Pools... Each With Size %d bytes",pool_count,max_size_of_pools);
                fprintf(fp,"You have %d No of Pools... Each With Size %d bytes",pool_count,max_size_of_pools);
        }
        fclose(fp);
}


void delete_chunk_contents()
{

        check_log_files();

        FILE*fp;
        fp=fopen(filename,"a+");

        int plno,chno,i,j,k,l,sum=0,chksize;
        printf("You have %d No of Pools\n",pool_count);
        printf("\n Enter the Pool No: ");
        scanf("%d",&plno);
        printf("\n");

        if(plno>pool_count)
        {
                printf("Sorry No such Pool is Available\n\n");
        }
     else
        {
                printf("\n Enter the Chunk No to delete its contents: ");
                scanf("%d",&chno);
                printf("\n");

                temp=start;
                for(i=1;(i<plno)&&(temp!=NULL);i++)
                {
                        temp=temp->next;
                }

                if(chno>temp->chunks_count)
                {
                        printf("Sorry No such Chunk is Available\n\n");
                }
                else
                {


                        fprintf(fp,"Memory Chunk De-allocation");
                        fprintf(fp,"\n---------------------------\n");
                        time(&now);
                        fprintf(fp,"Date & time is: %s\n",ctime(&now));

                        chksize=each_chunk_size[plno-1][chno-1];

                        for(j=0;j<(chno-1);j++)
                        {
                                sum=sum+(each_chunk_size[plno-1][j]);
                        }

                        for(l=sum,k=(sum+chksize);k<=temp->last;l++,k++)
                        {
                                temp->mem_chunk[l]=temp->mem_chunk[k];
                                fprintf(fp,"Contents in Pool No [%d] Chunk No [%d] is deleted\n",plno,chno);
                        }
                        fprintf(fp,"\n");
                        temp->last = (temp->last)-(chksize);
                        temp->available_mem =(temp->available_mem) + (chksize);

                        for(j=(chno-1);j<=(temp->chunks_count);j++)
                        {

      each_chunk_size[plno-1][j] = each_chunk_size[plno-1][j+1];
                        }
                        temp->chunks_count--;
                }
        }
fclose(fp);
}


void delete_pool()
{

        check_log_files();

        FILE*fp;
        int pno,i,j;
        printf("Enter the Pool No to be Deleted: ");
        scanf("%d",&pno);
        if(pno>pool_count)
        {
                printf("No such Pool available\n\n");
        }
        else
        {

                fprintf(fp,"Memory Pool De-allocation");
                fprintf(fp,"\n---------------------------\n");
                time(&now);
                fprintf(fp,"Date & time is: %s\n",ctime(&now));

                if(pno==1)
                {
                        temp=start;
                        start=start->next;
                        free(temp);

                        fprintf(fp,"De-allocating Memory Pool [%d] of Size [%d] bytes",i,max_size_of_pools);
                        fprintf(fp,"\nStaring Address of the Pool is[%d]\n",temp);
                }
                else if(pno==pool_count)
                {
                        for(temp=start;temp->next->next!=NULL;temp=temp->next);
                        ptr=temp->next;


                        fprintf(fp,"De-allocating Memory Pool [%d] of Size [%d] bytes",i,max_size_of_pools);
                        fprintf(fp,"\nStaring Address of the Pool is[%d]\n",ptr);

                        free(ptr);
                }
                else
                {
                        for(i=1,temp=start;(i<(pno-1))&&(temp!=NULL);i++)
                        {
                                temp=temp->next;
                        }
                        ptr=temp->next;
                        temp->next=ptr->next;

                        fprintf(fp,"De-allocating Memory Pool [%d] of Size [%d] bytes",i,max_size_of_pools);
                        fprintf(fp,"\nStaring Address of the Pool is[%d]\n",temp);

                        free(ptr);
                }

                for(i=(pno-1);i<(pool_count-1);i++)
                {
                        for(j=0;j<1024;j++)
                        {
                                each_chunk_size[i][j] = each_chunk_size[i+1][j];
                        }
                }

                pool_count--;
                printf("\n\n");
        }
}


void INT_signal_handler(int signum)
{

        check_log_files();

        FILE*fp;
        fp=fopen("Log.txt","a+");

  fprintf(fp,"Interrupt Handling");
        fprintf(fp,"\n------------------\n\n");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));
        fprintf(fp,"CTRL_C was pressed\n");
        fprintf(fp,"Caught Signal No: %d\n",signum);
        fclose(fp);
        printf("CTRL_C was pressed\n");
        printf("Caught Signal No: %d\n",signum);
        dealloc_all_pool();
        exit(signum);
}

void SEG_signal_handler(int signum)
{

        check_log_files();

        FILE*fp;
        fp=fopen("Log.txt","a+");
        time(&now);
        fprintf(fp,"Date & time is: %s\n",ctime(&now));
        fprintf(fp,"\nSEGMENTAION FAULT OCCURED\n");
        fclose(fp);
        printf("Caught Signal No: %d\n",signum);
        printf("Segmentaion Fault Occured\n");
        dealloc_all_pool();

        exit(signum);
}

int main()
{
        int menu,choice1;
        int i,ret,confirm;
        pthread_t tid;
        signal(SIGINT, INT_signal_handler);
        signal(SIGSEGV, SEG_signal_handler);
        printf("\n                  Welcome to Memory Management System\n");
        printf("                  ------------------------------------\n\n\n");

        do
        {

    printf("Do you want to\n");
                printf("---------------\n");
                printf("1) Manually enter the Max No of Pool & Pool Size... Press '1'\n");
                printf("2) Automatically assign default values..............Press '2'\n\n");
                printf("Enter your Choice: ");
                fflush(stdin);
                scanf("%d",&choice1);
                printf("\n\n");

                switch(choice1)
                {
                        case 1:
                                man_pool_alloc();
                                break;

                        case 2:
                                auto_pool_alloc();
                                break;

                        default:
                                printf("Invalid Selection\n\n");
                                break;

                }
                if(choice1==1)
                {
                        printf("To Confirm............... Press '1'\n");
                        printf("To Change Pool No & Size. Press '2'\n\n");
                        printf("Enter your Choice: ");
                        scanf("%d",&confirm);
                        printf("\n\n");
                        if(confirm==1)
                        {
                                add_mem_pool();
                        }
                        else if(confirm==2)
                        {
                                choice1=3;
                        }
                        else
                        {
                                printf("Invalid Selection\n\n");
                                choice1=3;

                  }
                }


        }while((choice1!=1) && (choice1!=2));


        while(1)
        {
                printf("Do you want to\n");
                printf("---------------\n");
                printf("1) Allocate Pools.................Press '1'\n");
                printf("2) Deallocate All Pools...........Press '2'\n");
                printf("3) Create Chunks..................Press '3'\n");
                printf("4) Memory Availability in Pools...Press '4'\n");
                printf("5) Memory Availability in Chunks..Press '5'\n");
                printf("6) Memomory Contents in the Pools.Press '6'\n");
                printf("7) Specific Chunk Contents........Press '7'\n");
                printf("8) Delete Specific Pool...........Press '8'\n");
                printf("9) Delete Specific Chunks.........Press '9'\n");
                printf("10) To Exit from the Program......Press '10'\n\n");
                printf("Enter your Choice: \n");
                fflush(stdin);
                scanf("%d",&menu);
                fflush(stdin);
                switch(menu)
                {
                                case 1 :
                                        add_individual_pool();
                                        break;

                                case 2 :
                                        dealloc_all_pool();
                                        break;

                                case 3:
                                        ret=pthread_create(&tid,NULL,&mem_chunk,NULL);
                                        pthread_join(tid,NULL);
                                        break;

                                case 4:
                                        mem_pool_availability();
                                        break;
                                        
                                case 5:
                                        mem_chunk_availability();
                                        break;

                                case 6:
                                        display_pool_contents();
                                        break;

                                case 7:
                                        display_chunk_contents();
                                        break;

                                case 8:
                                        delete_pool();
                                        break;

                                case 9:
                                        delete_chunk_contents();
                                        break;

                                case 10:
                                        dealloc_all_pool();
                                        exit(0);
                                        break;
                                default:
                                        printf("Invalid Selection\n\n");
                                        break;
                }
        }
return 0;
}

