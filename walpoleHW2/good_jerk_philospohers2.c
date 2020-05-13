#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>


// Change the constant below to change the number of philosophers
// coming to lunch...

#define PHILOSOPHER_COUNT 5

// Each philosopher is represented by one thread.  Each thread independenly
// runs the same "think/start eating/finish eating" program.
pthread_t philosopher[PHILOSOPHER_COUNT];

// Each chopstick gets one mutex.  If there are N philosophers, there are
// N chopsticks.  That's the whole problem.  There's not enough chopsticks 
// for all of them to be eating at the same time.  If they all cooperate, 
// everyone can eat.  If they don't... or don't know how.... well.... 
// philosophers are going to starve.

pthread_mutex_t chopstick[PHILOSOPHER_COUNT];

int Random_Int(int max)
   {  return((rand() % (max+1))); 
   }

void *philosopher_program(int philosopher_number)
   {  // In this version of the "philosopher program", the philosopher
      // will think and eat forever.
      
      int i_want_this;
      int i_want_this_too;
      
      while (1)
         { // Philosophers always think before they eat.  They need to 
           // build up a bit of hunger....
         
           usleep(1);  
           
           // That was a lot of thinking.... now hungry... this 
           // philosopher is a jerk.  He may not grap the chopsticks 
           // closest to him.  In fact, he may grab any two chopsticks at
           // the table.... because he is a jerk.
           
           i_want_this     = Random_Int(PHILOSOPHER_COUNT);
           while ((i_want_this_too = Random_Int(PHILOSOPHER_COUNT)) == i_want_this);
		   //still jerks in my mind
           printf ("Jerk Philosopher %d wants chopsticks %d and %d\n",
                    philosopher_number,
                    i_want_this,
                    i_want_this_too);
			//prevents cycle because while grabbing two diffrent chopsticks
			//if the first one is a higher value grab the lower one first
			//which is the recource hierarchy methodology 
			if(i_want_this < i_want_this_too)
			{
				//grabbing lower value first 
				pthread_mutex_lock(&chopstick[i_want_this]);
				//grabbing higher value next
				pthread_mutex_lock(&chopstick[i_want_this_too]);
			}
			else
			{
				//grabbing lower value first
				pthread_mutex_lock(&chopstick[i_want_this_too]);
				//grabbing higher value next
				pthread_mutex_lock(&chopstick[i_want_this]);
			}

           
           // Hurray, if I got this far I'm eating
		   //these people are jerks still in my mind for making me do this
           printf ("Jerk Philosopher %d is eating with chopsticks %d and %d\n",
                   philosopher_number,
                   i_want_this,
                   i_want_this_too);
           
           // I'm done eating.  Now put the chopsticks back on the table
           pthread_mutex_unlock(&chopstick[i_want_this]);
           pthread_mutex_unlock(&chopstick[i_want_this_too]);           
         } 

      return(NULL);
     }

int main()
  {  int i;

   srand(time(NULL));

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_init(&chopstick[i],NULL);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_create(&philosopher[i],NULL,(void *)philosopher_program,(void *)i);

   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_join(philosopher[i],NULL);


   for(i=0;i<PHILOSOPHER_COUNT;i++)
      pthread_mutex_destroy(&chopstick[i]);

   return 0;
   }

