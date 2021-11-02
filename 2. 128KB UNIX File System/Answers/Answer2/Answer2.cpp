#include<iostream>
#include<queue>
#include<bits/stdc++.h>
#include<vector>
#include<fstream>
using namespace std;

ifstream in;
int No_pages,No_frames;
int *Array;

/*This Function perform the "First In First Out" Page Replacement Method*/
int FIFO()
{
    queue<int> que;// temporary queue
    unordered_set<int> u_set; //unordered set
    int pagefault=0;//Initilizing the pagefault
    int temp;
    cout<<"/--First In First Out--/"<<endl;

    /*This Loop goes to the last page is referenced*/
    for(int i=0;i<No_pages;i++)
    {
        if(u_set.size()<No_frames)//if the page frames is not full
        {
            if(u_set.find(Array[i])==u_set.end())//checking if the referenced page is present in page frame or not. If not ,then insert the referenced page and pagefault increases; else no operation.
            {
                u_set.insert(Array[i]);

                que.push(Array[i]);
                pagefault++;
            }

        }
        else //if page frames are full
        {
            if(u_set.find(Array[i])==u_set.end())//checking if the referenced page is present in page frame or not, if not then page replacement policy starts and pagefault increases else no operation.
            {
                temp=que.front();
                que.pop();
                u_set.erase(temp);
                u_set.insert(Array[i]);
                que.push(Array[i]);
                pagefault++;
            }

        }

    }

    cout<<"#Pagefault : "<<pagefault<<endl<<endl<<endl;

    return pagefault;//retrunt the final pagefault

}


/*This Function perform the "Least Recent used" Replacement Method*/
int LRU()
{
    cout<<"/--Least Recent Used--/"<<endl;
    unordered_set<int> B;//unordered set
    unordered_map<int ,int> M;//unordered map for mapping the index with the referenced page, so that we knew the which page is used recently
    int pagefault=0;

     /*This Loop goes to the last page is referenced*/
    for(int i=0;i<No_pages;i++)
    {
        if(B.size()<No_frames)//if the page frames is not full
        {
            if(B.find(Array[i])==B.end())//checking if the referenced page is present in page frame or not. If not ,then insert the referenced page and pagefault increases; else no operation.
            {
                B.insert(Array[i]);
                pagefault++;

            }
            M[Array[i]]=i;
        }
        else//if page frames are full
        {
           if(B.find(Array[i])==B.end())//checking if the referenced page is present in page frame or not, if not then page replacement policy starts and pagefault increases else no operation.
           {
               int least_used=1000000, temp;

               for(auto j=B.begin();j!=B.end();j++)
               {
                   if(M[*j]<least_used)//checking which page is used least
                   {
                       least_used=M[*j];
                       temp=*j;
                   }

               }

                B.erase(temp);

                B.insert(Array[i]);

                pagefault++;
           }
           M[Array[i]]=i;
        }
    }

    cout<<"#Pagefault : "<<pagefault<<endl<<endl<<endl;


    return pagefault;//retrunt the final pagefault

}

int LFU()
{
    cout<<"/--Least Frequent Used--/"<<endl;
    unordered_set<int> B; //unordered set
    unordered_map<int ,int> M_i,M_f; //unordered Map ; using for maping index and frequency of reference pages
    int pagefault=0;


    /*This Loop goes to the last page is referenced*/
    for(int i=0;i<No_pages;i++)
    {

        if(B.size()<No_frames)//if the page frames is not full
        {
            if(B.find(Array[i])==B.end())//checking if the referenced page is present in page frame or not. If not ,then insert the referenced page and pagefault increases;
            {
                B.insert(Array[i]);
                pagefault++;
                M_i[Array[i]]=i;
                M_f[Array[i]]=1;

            }
            else //If referenced page is present in the Page Frame then the frequency of the page is increased by 1
            {
                M_f[Array[i]]++;
            }

        }
        else //if page frames are full
        {
           if(B.find(Array[i])==B.end())//checking if the referenced page is present in page frame or not, if not ,then page replacement policy starts and pagefault increases.
           {
               int least_used=1000000,least_freq=100000, temp;

               for(auto j=B.begin();j!=B.end();j++)
               {

                   if(M_f[*j]<least_freq)//checking which page in the page frame has least frequency
                   {
                       least_freq=M_f[*j];
                       temp=*j;
                   }
                    else if(M_f[*j] == least_freq  && M_i[*j] < M_i[temp])//If two pages have same frequency then we check which one come first
                   {
                   		temp = *j;
				   }
               }

                B.erase(temp);

                B.insert(Array[i]);
                M_i[Array[i]]=i;
                M_f[Array[i]]=1;

                pagefault++;
           }
           else //If referenced page is present in the Page Frame then the frequency of the page is increased by 1
            {
                M_f[Array[i]]++;
            }
        }

    }

    cout<<"#Pagefault : "<<pagefault<<endl<<endl<<endl;

    return pagefault;//Final Pagefault
}

int main(int argc,char* argv[])
{
    in.open(argv[1]);
    if(in.is_open())
    {
        in>>No_pages;
        in>>No_frames;
        if(No_frames<=0 || No_pages<=0)
        {
            cout<<"Number of pages/frames must be positive.";
            return 0;
        }
        Array= new int[No_pages];
        for(int i=0;i<No_pages;i++)
        {
            in>>Array[i];
        }
    }
    else{
        cout<<"File not open"<<endl;
    }

    cout<<"\n\n";

    if(argc >= 3) //If only one page replacement policy is given
     {
        switch(argv[2][0])
        {
            case 'F': FIFO();
                        //flag=1;
                    break;
            case 'L':if(argv[2][1]=='F')
                        {
                            LFU();
                            //flag=1;
                        }
                        else if(argv[2][1]=='R')
                        {
                            LRU();
                          //  flag=1;
                        }
                        else
                            cout<<"Wrong Input"<<endl;
                    break;


        }
     }

     if(argc >= 4)//If two page replacement policies are given
      {
         switch(argv[3][0])
        {
            case 'F': FIFO();
                    break;
            case 'L':if(argv[3][1]=='F')
                        {
                            LFU();
                        }
                        else if(argv[3][1]=='R')
                        {
                            LRU();
                        }

                        else
                            cout<<"Wrong Input"<<endl;
                    break;


        }
      }

       if(argc >= 5)//If all three page replacement policies are given
       {
        switch(argv[4][0])
        {
            case 'F': FIFO();
                    break;
            case 'L':if(argv[4][1]=='F')
                        {
                            LFU();
                        }
                        else if(argv[4][1]=='R')
                         {
                            LRU();
                        }
                        else
                            cout<<"Wrong Input"<<endl;
                    break;
        }
       }


    if(argc==2) //If no Algorithm is specified in the input we perform all the Page Replacement policy in the given manner;
    {
        FIFO();
        LFU();
        LRU();
    }
    return 0;
}
