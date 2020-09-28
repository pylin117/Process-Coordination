#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */ 
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string>     /* String handling */
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <vector>
#include <iterator>
using namespace std;
void *work ( void* ptr );
pthread_mutex_t mutex;
int count=0;
int currentTime=0;
int studentClock=0;
int movieClock=0;
int YClock=0,YFinish=0;
int TYClock=0,TYFinish=0;
vector<int> chair;
bool ProfTY=true;
bool TAY=true;
bool startY=false;
bool startTY=false;
bool startMovie=false;
vector<int> check; //確認是否和Y助教討論過，0=還沒，1=討論過
int TYTalking=0;
int countTY=0;
//srand(NULL);
struct thdata
{
  int thread_no;
};
int main()
{
	srand(0);
	pthread_t threads[52];
  pthread_mutex_init(&mutex, NULL);
    for(int i=0;i<52;i++)
    {
        thdata *tdata=new thdata();
        tdata->thread_no=i;
        pthread_create (&threads[i],NULL,work,tdata);
    }
    for(int i=0;i<52;i++)
        pthread_join(threads[i],NULL);
    pthread_mutex_destroy(&mutex);
    exit(0);
}


void SitAndWait(int currentTime, int index)
{
  chair.push_back(index);
  check.push_back(0);
  cout<<currentTime<<"ms -- Student ";
  if(index<10){cout<<"0";}
  cout<<index<<": enter & wait"<<endl;
}

void WatchMovie(int currentTime, int index)
{
  cout<<currentTime<<"ms -- Student ";
  if(index<10){cout<<"0";}
  cout<<index<<": watch movie "<<movieClock<<"ms"<<endl;
}

void WaitForTY(int currentTime, int index)
{
  chair.push_back(index);
  check.push_back(1);
  cout<<currentTime<<"ms -- Student ";
  if(index<10){cout<<"0";}
  cout<<index<<": wait for Prof. TY"<<endl;
}

void WaitAndWatchMovie(int currentTime, int index)
{
  cout<<currentTime<<"ms -- Student ";
  if(index<10){cout<<"0";}
  cout<<index<<": wait & watch movie "<<movieClock<<"ms"<<endl;
}

void *work ( void* ptr )
{
    thdata *tdata=(thdata *)ptr;
    int index=tdata->thread_no-1;
    int nextTime=0;
    bool done=false;
    int movieFinish=0;

    if(tdata->thread_no==0)
    {
      pthread_mutex_lock(&mutex);
      cout<<"0ms -- Prof.TY: rest"<<endl;
      pthread_mutex_unlock(&mutex);
      count++;
      done=true;
    }
    else if(tdata->thread_no==1)
    {
      pthread_mutex_lock(&mutex);
      cout<<"0ms -- TA Y: rest"<<endl;
      pthread_mutex_unlock(&mutex);
      count++;
      done=true;
    }
    else
    {
      while(true)
      {
        pthread_mutex_lock(&mutex);
        if(countTY==50)
        {
          pthread_mutex_unlock(&mutex);
          break;
        }

        studentClock=(rand()%6)+5; //同學到達的間隔時間是5~10 ms
        movieClock=(rand()%21)+30; //離開30~50 ms看電影
        YClock=(rand()%21)+10; //與洋助教討論10~30 ms
        TYClock=(rand()%51)+50; //與TY教授討論50~100 ms
        
        nextTime=currentTime+studentClock;
        if(tdata->thread_no==count) //輪到第幾個同學
        {
          if(!startY&&index==1)
          {
            chair.push_back(index);
            check.push_back(0);
          }
          if(startY&&YFinish<=nextTime) //Y助教和同學討論完討論完
          {
            TAY=true;
            if(ProfTY) //若TY教授有空直接進去討論
            {
              if(TYFinish==0)
                TYFinish=YFinish;
              ProfTY=false;
              if(check[0]==1)
              {
                cout<<TYFinish<<"ms -- Prof. TY: discuss with Student ";
                if(chair[0]<10){cout<<"0";}
                cout<<chair[0]<<" "<<TYClock<<" ms"<<endl; 
                TYTalking=chair[0];
                YFinish=TYFinish;  
                TYFinish+=TYClock;
              }

              else
              {
                cout<<YFinish<<"ms -- Prof. TY: discuss with Student ";
                if(chair[0]<10){cout<<"0";}
                cout<<chair[0]<<" "<<TYClock<<" ms"<<endl; 
                TYTalking=chair[0]; 
                TYFinish=YFinish+TYClock; 
              }
              
              chair.erase(chair.begin());
              check.erase(check.begin()); 
              
              startTY=true;
            }
            else if(chair.size()-1<6) //若椅子有位坐上去
            {
              if(check[0]==0)
              {
                WaitForTY(YFinish,chair[0]);
                chair.erase(chair.begin());
                check.erase(check.begin());
              }
            }
            else //都沒有就去看電影
            {
              WaitAndWatchMovie(YFinish,chair[0]);
              chair.erase(chair.begin());
              check.erase(check.begin());
            }
          }

          if(startTY&&!done&&TYFinish<=nextTime) //TY教授和同學討論完
          {
            countTY++;
            ProfTY=true;
            cout<<TYFinish<<"ms -- Student ";
            if(TYTalking<10){cout<<"0";}
            cout<<TYTalking<<": leave"<<endl;
            done=true;
          }

          if(check[0]==1&&!ProfTY)
            currentTime=TYFinish;
          if(TAY) //TAY有空
          {
            currentTime+=studentClock;
            if(YFinish==0)
              YFinish=currentTime;
            //if(check[0]==1&&!ProfTY){;}
            //Y助教
            if(check[0]!=1)
            {
              cout<<YFinish<<"ms -- Student ";
              if(chair[0]<10){cout<<"0";}
              cout<<chair[0]<<": enter"<<endl;
              cout<<YFinish<<"ms -- TA Y: discuss with Student ";
              if(chair[0]<10){cout<<"0";}
              cout<<chair[0]<<" "<<YClock<<" ms"<<endl;
              TAY=false;
              YFinish+=YClock;
            }
            startY=true;
          }
          if(index==1){}
          else if(chair.size()<6) //還有椅子坐
          {
            currentTime=nextTime;
            SitAndWait(currentTime,index);
          }
          else //沒有椅子坐
          {
            currentTime=nextTime;
            movieFinish=currentTime+movieClock; //看完電影的時間
            WatchMovie(currentTime,index);
            startMovie=true;
          }

          count++;
        }

        pthread_mutex_unlock(&mutex);

        if(tdata->thread_no!=count) //還沒輪到的話過1ms再檢查一次
          usleep(0.001);
      }
    }

    pthread_exit(0); /* exit */
}
