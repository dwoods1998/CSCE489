/*************************************************************************************
 * babyyoda - used to test your semaphore implementation and can be a starting point for
 *			     your store front implementation
 *
 *************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include "Semaphore.h"

// Semaphores that each thread will have access to as they are global in shared memory
Semaphore *empty = NULL;
Semaphore *full = NULL;

pthread_mutex_t buf_mutex;

int buffer = 0;
int consumed = 0;


/*************************************************************************************
 * producer_routine - this function is called when the producer thread is created.
 *
 *			Params: data - a void pointer that should point to an integer that indicates
 *							   the total number to be produced
 *
 *			Returns: always NULL
 *
 *************************************************************************************/

void *producer_routine(void *data) {

	time_t rand_seed;
	srand((unsigned int) time(&rand_seed));

	// The current serial number (incremented)
	unsigned int serialnum = 1;
	
	// We know the data pointer is an integer that indicates the number to produce
	int left_to_produce = *((int *) data);

	// Loop through the amount we're going to produce and place into the buffer
	while (left_to_produce > 0) {
		printf("Producer wants to put Yoda #%d into buffer...\n", serialnum);

		// Semaphore check to make sure there is an available slot
		full->wait();

		// Place item on the next shelf slot by first setting the mutex to protect our buffer vars
		pthread_mutex_lock(&buf_mutex);

		buffer = serialnum;
		serialnum++;
		left_to_produce--;

		pthread_mutex_unlock(&buf_mutex);
		
		printf("   Yoda put on shelf.\n");
		
		// Semaphore signal that there are items available
		empty->signal();

		// random sleep but he makes them fast so 1/20 of a second
		usleep((useconds_t) (rand() % 200000));
	
	}
	return NULL;
}


/*************************************************************************************
 * consumer_routine - this function is called when the consumer thread is created.
 *
 *       Params: data - a void pointer that should point to a boolean that indicates
 *                      the thread should exit. Doesn't work so don't worry about it
 *
 *       Returns: always NULL
 *
 *************************************************************************************/

void *consumer_routine(void *data) {
	(void) data;

	bool quitthreads = false;

	while (!quitthreads) {
		printf("Consumer wants to buy a Yoda...\n");

		// Semaphore to see if there are any items to take
		empty->wait();

		// Take an item off the shelf
		pthread_mutex_lock(&buf_mutex);
	
		printf("   Consumer bought Yoda #%d.\n", buffer);
		buffer = 0;
		consumed++;
	
		pthread_mutex_unlock(&buf_mutex);

		// Consumers wait up to one second
		usleep((useconds_t) (rand() % 1000000));

		full->signal();
	}
	printf("Consumer goes home.\n");

	return NULL;	
}


/*************************************************************************************
 * main - Standard C main function for our storefront. 
 *
 *		Expected params: pctest <num_consumers> <max_items>
 *				max_items - how many items will be produced before the shopkeeper closes
 *
 *************************************************************************************/

int main(int argv, const char *argc[]) {

	// Get our argument parameters
	if (argv < 2) {
		printf("Invalid parameters. Format: %s <max_items>\n", argc[0]);
		exit(0);
	}

	// User input on the size of the buffer
	int num_produce = (unsigned int) strtol(argc[1], NULL, 10);


	printf("Producing %d today.\n", num_produce);
	
	// Initialize our semaphores
	empty = new Semaphore(0);
	full = new Semaphore(1);

	pthread_mutex_init(&buf_mutex, NULL); // Initialize our buffer mutex

	pthread_t producer;
	pthread_t consumer;

	// Launch our producer thread
	pthread_create(&producer, NULL, producer_routine, (void *) &num_produce);

	// Launch our consumer thread
	pthread_create(&consumer, NULL, consumer_routine, NULL);

	// Wait for our producer thread to finish up
	pthread_join(producer, NULL);

	printf("The manufacturer has completed his work for the day.\n");

	printf("Waiting for consumer to buy up the rest.\n");

	// Give the consumers a second to finish snatching up items
	while (consumed < num_produce)
		sleep(1);

	// Now make sure they all exited
//	for (unsigned int i=0; i<NUM_CONSUMERS; i++) {
//		pthread_join(consumers[i], NULL);
//	}

	// We are exiting, clean up
	delete empty;
	delete full;		

	printf("Producer/Consumer simulation complete!\n");

}
