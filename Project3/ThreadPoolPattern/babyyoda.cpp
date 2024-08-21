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
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <random>
#include "Bounded_Buffer.h"
#include "ThreadPool.h"
#include <atomic>

std::atomic<int> active_producers(0);
/*************************************************************************************
 * producer_routine - produces items and deposits them into the buffer
 *
 *			Params: data pointer to an integer indicating the total number of items to
 *				produce
 * 
 *			Returns: always nullptr
 *************************************************************************************/
void producer_task(int left_to_produce, int producerID, BoundedBuffer* buffer) {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 200000);

	++active_producers;

	while (left_to_produce > 0) {
		//Deposit into buffer
		buffer->Deposit(producerID);
		left_to_produce--;
		//std::cout<<active_producers<< "active producers\n";
		//sleep
		std::this_thread::sleep_for(std::chrono::microseconds(dis(gen)));
	}

	--active_producers;
}


/*************************************************************************************
 * consumer_routine - consumes items from the buffer
 *
 *       Params: data pointer to an integer indicating the consumer ID
 *                      
 *       Returns: always nullptr
 *
 *************************************************************************************/

void consumer_task(int consumerID, BoundedBuffer* buffer) {

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100000);

	while (true){
		std::cout <<" Consumer #" << consumerID << " wants to buy a yoda.\n";
		//retrieve yoda from buffer
		Item* yoda = buffer->Retrieve(consumerID);

		if(yoda == nullptr && buffer->isDone()){
			break;//Exit if there are none 
		}

		if(yoda != nullptr){
			delete yoda;
		}

		// random sleep
		std::this_thread::sleep_for(std::chrono::microseconds(dis(gen)));
	}
	
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
	if (argc != 5) {
		std::cerr <<"Invalid parameters. Format: " << argv[0] << " <buffer_size> <num_producers> <num_consumers> <max_items>\n";
		return EXIT_FAILURE;
	}

	int buffer_size = std::stoi(argv[1]);
	int num_producers = std::stoi(argv[2]);
	int num_consumers = std::stoi(argv[3]);
	int num_produce = std::stoi(argv[4]);
	
	//input validation
	if (buffer_size <= 0 || num_producers <= 0 || num_consumers<= 0 || num_produce < 0){
		std::cerr << "Invalid paramaters. Buffer size, number of producers and number of consumers must be >=1; number to produce must be >=0.\n";
		return EXIT_FAILURE;
	}

	std::cout << "Producing "<<num_produce<< " yodas today.\n";
	

	//Initialize bounded buffer
	BoundedBuffer buffer(buffer_size);
	
	ThreadPool threadPool(num_producers + num_consumers);

	//Items per producer
	int itemsPerProducer = num_produce / num_producers;
	int remaining_items = num_produce % num_producers;

	// std::cout<<"test1\n";
	//Create producer tasks
	for (int i = 0; i < num_producers; ++i){
		int produce_count = itemsPerProducer + (i < remaining_items ? 1 : 0);
		threadPool.enqueue([&buffer, i, produce_count]() {
			producer_task(produce_count, i+1, &buffer);
		});
	}
	
	// std::cout<<"test2\n";
	//Create Consumer threads
	for (int i = 0; i < num_consumers; ++i){
		threadPool.enqueue([i, &buffer](){
			consumer_task(i + 1, &buffer);
		});
	}
	
	std::cout<<"\n";

	while (active_producers > 0){
		//causes the program to wait before executing the next line
	}
	buffer.setDone();

	std::cout << "Producer/Consumer simulation complete!\n";
	return EXIT_SUCCESS;
}
