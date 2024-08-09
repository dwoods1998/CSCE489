/*************************************************************************************
 * babyyoda - A program that uses a bounded buffer and semaphores to simulate 
 * a Producer to multiple consumer simulation.
 * 
 * I took the code that was provided for the storefront and modified it to fit my bounded
 * buffer class.  In doing so, i had to make several changes so that it would all
 * successfully inegrate and cooperate with eachother. Additionally, I utilized chatGPT
 * for help with the communication between the threads and for general debugging/optimization
 *
 *************************************************************************************/

#include <iostream>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include "Bounded_Buffer.h"

//Global buffer point
BoundedBuffer* buffer = nullptr;

/*************************************************************************************
 * producer_routine - produces items and deposits them into the buffer
 *
 *			Params: data pointer to an integer indicating the total number of items to
 *				produce
 * 
 *			Returns: always nullptr
 *************************************************************************************/
void* producer_routine(void* data) {
	unsigned int serialnum = 1;
	int left_to_produce = *(int*)data;

	time_t rand_seed;
	srand((unsigned int) time(&rand_seed));

	while (left_to_produce > 0) {
		std::cout << "Producer wants to put Yoda #" << serialnum << " into buffer...\n";

		// create new yoda
		Item* yoda = new Item("Yoda #" + std::to_string(serialnum));

		//Deposit into buffer
		buffer->Deposit(yoda);
		
		std::cout <<"  Yoda #"<< serialnum << " put on shelf.\n";

		serialnum++;
		left_to_produce--;

		// random sleep but he makes them fast so 1/20 of a second
		usleep((useconds_t) (rand() % 200000));
	}
	return nullptr;
}


/*************************************************************************************
 * consumer_routine - consumes items from the buffer
 *
 *       Params: data pointer to an integer indicating the consumer ID
 *                      
 *       Returns: always nullptr
 *
 *************************************************************************************/

void* consumer_routine(void* data) {
	int consumerID = *(int*)data;

	time_t rand_seed;
	srand((unsigned int) time(&rand_seed));

	while (true){
		std::cout << "Consumer #" << consumerID << " wants to buy a Yoda...\n";

		
		//retrieve yoda from buffer
		Item* yoda = buffer->Retrieve();

		if(yoda == nullptr){
			break;//Exit if there are none
			 
		}

		
		std::cout <<"Consumer #" << consumerID << " bought " << yoda->GetContent() << ".\n";
		delete yoda;

		// random sleep
		usleep((useconds_t)(rand() % 1000000));
	}
	
	return nullptr;
}


/*************************************************************************************
 * main - Standard C main function for our storefront. 
 *
 *		Expected params: <buffer_size> <num_consumers> <max_items>
 *				
 *				buffer_size - set the size of the buffer
 * 				num_consumers - how many consumers the simulation will use
 * 				max_items - how many items will be produced before the shopkeeper closes
 *************************************************************************************/

int main(int argc, const char* argv[]) {

	//Get argument parameters
	if (argc != 4) {
		std::cerr <<"Invalid parameters. Format: " << argv[0] << " <buffer_size> <num_consumers> <max_items>\n";
		return EXIT_FAILURE;
	}

	int buffer_size = std::stoi(argv[1]);
	int num_consumers = std::stoi(argv[2]);
	int num_produce = std::stoi(argv[3]);
	
	//input validation
	if (buffer_size <= 0){
		std::cerr << "Buffer size must be an integer >= 1\n";
		return EXIT_FAILURE;
	}else if (num_consumers <= 0){
		std::cerr << "Number of consumers must be an integer >= 1\n";
		return EXIT_FAILURE;
	}else if (num_produce < 0){
		std::cerr << "Maximum items to produce must be an integer >= 0\n";
		return EXIT_FAILURE;
	}

	std::cout << "Producing "<<num_produce<< " yodas today.\n";
	

	//Initialize bounded buffer
	buffer = new BoundedBuffer(buffer_size); //sets buffer to 10
	
	pthread_t producer;
	std::vector<pthread_t> consumers(num_consumers);
	std::vector<int> consumer_ids(num_consumers);

	//Create producer thread
	if(pthread_create(&producer, nullptr, producer_routine, &num_produce) != 0){
		std::cerr << "Error creating producer thread\n";
		return EXIT_FAILURE;
	}

	//Create Consumer threads
	for (int i = 0; i < num_consumers; ++i){
		consumer_ids[i] = i + 1;
		if(pthread_create(&consumers[i], nullptr, consumer_routine, &consumer_ids[i]) != 0){
			std::cerr<< "Error creating consumer thread\n" << i + 1 << "\n";
			return EXIT_FAILURE;
		}
	}
	
	//wait for producer to finish
	if(pthread_join(producer, nullptr) != 0){
		std::cerr << "Error joining producer thread\n";
		return EXIT_FAILURE;
	}

	buffer->setDone();//notify production is complete

	std::cout << "The manufacturer has completed his work for the day.\n";

	//cean up threads
	int tempID = 1;
	if (num_consumers > buffer_size){ // this handles an error where the threads hangup if consumers is > buffer size
		for (pthread_t& consumer : consumers){
			pthread_cancel(consumer);
			std::cout <<"Consumer #" <<tempID<<" left for the day.\n";
			tempID += 1;
		}
	}else{
		for (pthread_t& consumer : consumers){
			pthread_join(consumer, nullptr);
			std::cout <<"Consumer #" <<tempID<<" left for the day.\n";
			tempID += 1;
		}

	}
	
	// We are exiting, clean up
	delete buffer;
			

	std::cout << "Producer/Consumer simulation complete!\n";
	return EXIT_SUCCESS;
}
