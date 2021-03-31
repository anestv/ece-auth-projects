/*
 *	Title	: Demo Producer/Consumer.
 *	Short	: A solution to the producer consumer problem using pthreads.
 *	Author	: Andrae Muys
 *	Date	: 18 September 1997
 *
 *	Revised	: Anestis V., March 2021
 */

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

#define QUEUESIZE 40 //TODO how much?
#define LOOP 1000

void *producer (void *args);
void *consumer (void *args);


typedef struct {
  void * (*work)(void *);
  void * arg;
  long timeSubmitted_us;
} workFunction;

typedef struct {
  workFunction buf[QUEUESIZE];
  long head, tail;
  int full, empty;
  pthread_mutex_t *mut;
  pthread_cond_t *notFull, *notEmpty;
} queue;

queue *queueInit ();
void queueDelete (queue *q);
void queueAdd (queue *q, workFunction in);
void queueDel (queue *q, workFunction *out);

void* worker1(void * arg);
void* worker2(void * arg);

void printTime(int time_us);

int totalWorkfunctions; // gia na kseroume pote teleiwsame


int main (int argc, char **argv)
{
  if (argc != 3){
    fprintf (stderr, "Program must be called: %s [numProducers] [numConsumers]\n", argv[0]);
    exit (1);
  }
  
  const int p = atoi(argv[1]); // num of producers
  const int q = atoi(argv[2]); // num of consumers
  int i;
  
  totalWorkfunctions = p * LOOP;
  
  queue *fifo = queueInit();
  pthread_t *pro = malloc(p * sizeof(pthread_t));
  pthread_t *con = malloc(q * sizeof(pthread_t));

  if (fifo ==  NULL) {
    fprintf (stderr, "main: Queue Init failed.\n");
    exit (1);
  }
  
  for (i = 0; i < p; i++)
    pthread_create(&pro[i], NULL, producer, fifo);
  
  for (i = 0; i < q; i++)
    pthread_create(&con[i], NULL, consumer, fifo);
  
  printf("Finished creating p+q threads\n");
  
  for (i = 0; i < p; i++)
    pthread_join (pro[i], NULL);
  
  for (i = 0; i < q; i++)
    pthread_join (con[i], NULL);
  
  queueDelete (fifo);
  free(pro);
  free(con);

  return 0;
}

void *producer (void *q)
{
  queue *fifo = (queue *)q;
  workFunction w;
  struct timeval tv;


  for (int i = 0; i < LOOP; i++) {
  	//printf("prod %d\n", i);
	
	// create a workFunction
	
	gettimeofday(&tv, NULL);
	w.timeSubmitted_us = tv.tv_sec * 1000000 + tv.tv_usec;
	
	w.work = (i % 2) ? &worker1 : &worker2;
	float * arg = malloc(sizeof(float));
	*arg = (i % 12) / 17;
	w.arg = arg;
	
    pthread_mutex_lock (fifo->mut);
    while (fifo->full) {
      //printf ("producer: queue FULL.\n");
      pthread_cond_wait (fifo->notFull, fifo->mut);
    }
    
    queueAdd (fifo, w);
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notEmpty);
  }
  
  return (NULL);
}

void *consumer (void *q)
{
  queue *fifo = (queue *)q;
  workFunction w;
  struct timeval tv;

  while (1) {
  	//printf("cons\n");
  	
    pthread_mutex_lock (fifo->mut);
    while (fifo->empty) {
      //printf ("consumer: queue EMPTY.\n");
      pthread_cond_wait (fifo->notEmpty, fifo->mut);
    }
    queueDel (fifo, &w);
    //pthread_mutex_unlock (fifo->mut);
    //pthread_cond_signal (fifo->notFull);
	
	// deal with received element
	
	gettimeofday(&tv, NULL);
	int timeWaiting_us = tv.tv_sec * 1000000 + tv.tv_usec - w.timeSubmitted_us;
	printTime(timeWaiting_us); // ή να τα στειλω στο main thread με αλλον τροπο
	
    pthread_mutex_unlock (fifo->mut);
    pthread_cond_signal (fifo->notFull);
	
	
	w.work(w.arg);
	free(w.arg);
  }
  
  return (NULL);
}

void* worker1(void * arg)
{
  float* x = arg;
  float y;
  
  for (int i = 1; i <= 10; i++)
    y = sinf(*x + i*M_PI/5);
  
  return NULL;
}
  
void* worker2(void * arg)
{
  float* x = arg;
  float y;
  
  for (int i = 1; i <= 10; i++)
    y = tanhf(*x + i*M_PI/5);
  
  return NULL;
}

void printTime(int time_us)
{
  static int timesCalled = 0;
  static double sum = 0, sumSqr = 0;
  
  printf("%d\n", time_us);
  
  sum += time_us;
  sumSqr += time_us * time_us;
  
  if (++timesCalled >= totalWorkfunctions){
    
    sum /= timesCalled;
    printf("Mean time (us): %.1f\n", sum);

    sumSqr = (sumSqr / timesCalled) - (sum * sum);
    printf("Variance (us): %.1f\n", sumSqr);
    
    fflush(stdout);
    
    // TODO stop all threads
    exit(0);
  }
}


queue *queueInit ()
{
  queue *q = malloc(sizeof (queue));
  if (q == NULL) return (NULL);

  q->empty = 1;
  q->full = 0;
  q->head = 0;
  q->tail = 0;
  q->mut = (pthread_mutex_t *) malloc (sizeof (pthread_mutex_t));
  pthread_mutex_init (q->mut, NULL);
  q->notFull = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notFull, NULL);
  q->notEmpty = (pthread_cond_t *) malloc (sizeof (pthread_cond_t));
  pthread_cond_init (q->notEmpty, NULL);
	
  return q;
}

void queueDelete (queue *q)
{
  pthread_mutex_destroy (q->mut);
  free (q->mut);	
  pthread_cond_destroy (q->notFull);
  free (q->notFull);
  pthread_cond_destroy (q->notEmpty);
  free (q->notEmpty);
  free (q);
}

void queueAdd (queue *q, workFunction in)
{
  q->buf[q->tail] = in;
  q->tail++;
  if (q->tail == QUEUESIZE)
    q->tail = 0;
  if (q->tail == q->head)
    q->full = 1;
  q->empty = 0;

}

void queueDel (queue *q, workFunction *out)
{
  *out = q->buf[q->head];

  q->head++;
  if (q->head == QUEUESIZE)
    q->head = 0;
  if (q->head == q->tail)
    q->empty = 1;
  q->full = 0;

}
