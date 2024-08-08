/*************************************************************************************
 * babyyoda - used to test your semaphore implementation and can be a starting point for
 *			     your store front implementation
 *
 *************************************************************************************/

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <vector>
#include "Bounded_Buffer.h"

//Global buffer point
BoundedBuffer* buffer = nullptr;
int num_produce = 0;

/*************************************************************************************
 * producer_routine - this function is called when the producer thread is created.
 *
 *			Params: data - a void pointer that should point to an integer that indicates
 *							   the total number to be produced
 *
 *			Returns: always NULL
 *
 *************************************************************************************/

void* producer_routine(void* data) {
	unsigned int serialnum = 1;
	int left_to_produce = *(int*)data;

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
 * consumer_routine - this function is called when the consumer thread is created.
 *
 *       Params: data - a void pointer that should point to a boolean that indicates
 *                      the thread should exit. Doesn't work so don't worry about it
 *
 *       Returns: always NULL
 *
 *************************************************************************************/

void* consumer_routine(void* data) {
	int consumerID = *(int*)data;

	while (true){
		//std::cout << "Consumer wants to buy a Yoda...\n";

		
		//retrieve yoda from buffer
		Item* yoda = buffer->Retrieve();

		if(yoda == nullptr){
			break; //Exit if there are none
		}

		
		std::cout <<"  Consumer #" << consumerID << " bought " << yoda->GetContent() << ".\n";
		delete yoda;

		// random sleep
		usleep((useconds_t)(rand() % 1000000));
	}
	return nullptr;
}


/*************************************************************************************
 * main - Standard C main function for our storefront. 
 *
 *		Expected params: pctest <num_consumers> <max_items>
 *				max_items - how many items will be produced before the shopkeeper closes
 *
 *************************************************************************************/

int main(int argc, const char* argv[]) {

	//Get our argument parameters
	if (argc != 4) {
		std::cerr <<"Invalid parameters. Format: " << argv[0] << " <buffer_size> <num_consumers> <max_items>\n";
		return EXIT_FAILURE;
	}

	int buffer_size = std::stoi(argv[1]);
	int num_consumers = std::stoi(argv[2]);
	num_produce = std::stoi(argv[3]);


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
	//std::cout << "buffer set to done\n";

	//Wait for all consumers to finish
	for (pthread_t& consumer : consumers){
		// std::cout << "**waiting for consumer thread to finish\n";
		// if(pthread_join(consumer, nullptr) != 0){
		// 	std::cerr << "Error joining consumer thread\n";
		// 	return EXIT_FAILURE;
			
		// }
		pthread_cancel(consumer);
		// std::cout <<"****consumer joined\n";
	}
	

	std::cout << "The manufacturer has completed his work for the day.\n";

	
	// We are exiting, clean up
	delete buffer;
			

	std::cout << "Producer/Consumer simulation complete!\n";
	return EXIT_SUCCESS;
}
