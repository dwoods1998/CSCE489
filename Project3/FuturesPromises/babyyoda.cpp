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
#include "BoundedBuffer.h"
#include "ThreadPool.h"
#include <atomic>
#include <condition_variable>
#include <mutex>

//atomic variable for active producers
std::atomic<int> active_producers(0);

//CV & mutex for when producers are done
bool doneProducing= false;
std::mutex cv_m;
std:: condition_variable cv;

//Mutex for finished output
std::mutex outputMTX;

/*************************************************************************************
 * producer_task - Produces items and deposits them into the buffer
 *
 * Parameters:
 *   left_to_produce - Number of items left to produce
 *   producerID - ID of the producer
 *   buffer - Pointer to the BoundedBuffer object
 *
 * Returns: None
 *************************************************************************************/
void producer_task(int left_to_produce, int producerID, BoundedBuffer* buffer) {
	//Sets up a random sleep duration
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 200000);

	++active_producers; //increment active producer count

	while (left_to_produce > 0) {
		buffer->Deposit(producerID); //Deposit an item into buffer
		left_to_produce--;
		std::this_thread::sleep_for(std::chrono::microseconds(dis(gen))); //sleep for random distribution
	}

	--active_producers; //decrement active producer count

    //notify main thread producers are done
    if (active_producers == 0) {
		std::lock_guard<std::mutex> lock(cv_m);
        doneProducing = true; // set flag indicating production is done
        cv.notify_all(); //notify all waiting threads
	}

	std::lock_guard<std::mutex> lock(outputMTX);
	std::cout << "  Producer #"<< producerID << " has finished for the day.\n";
}

/*************************************************************************************
 * consumer_task - Consumes items from the buffer
 *
 * Parameters:
 *   consumerID - ID of the consumer
 *   buffer - Pointer to the BoundedBuffer object
 *
 * Returns: None
 *************************************************************************************/
void consumer_task(int consumerID, BoundedBuffer* buffer) {
	//Sets up random sleep duration
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 100000);

	while (true){
		std::cout <<"Consumer #" << consumerID << " wants to buy a yoda.\n";
		
		Item* yoda = buffer->Retrieve(consumerID);//retrieve yoda from buffer

		//If not item is retrieved, wait for production to be done
		if (yoda == nullptr) { //check if production is done
            std::unique_lock<std::mutex> lock(cv_m);
            cv.wait(lock, [] { return doneProducing; });//wait for production to finish
            if (buffer->isDone()) {//check if buffer is done
                break; //exit if buffer is done
            }
        } else {
            delete yoda; //delete the consumed item
		}
		
		std::this_thread::sleep_for(std::chrono::microseconds(dis(gen)));// random sleep
	}

	std::lock_guard<std::mutex> lock(outputMTX);
	std::cout << "  Consumer #"<< consumerID << " has left for the day.\n";
}

/*************************************************************************************
 * main - Entry point for the storefront simulation
 *
 * Parameters:
 *   argc - Argument count
 *   argv - Argument vector containing the parameters
 *
 * Returns: EXIT_SUCCESS or EXIT_FAILURE
 *************************************************************************************/

int main(int argc, const char* argv[]) {

	//validate command-line arguments
	if (argc != 5) {
		std::cerr <<"Invalid parameters. Format: " << argv[0] << " <buffer_size> <num_producers> <num_consumers> <max_items>\n";
		return EXIT_FAILURE;
	}

	int buffer_size = std::stoi(argv[1]);
	int num_producers = std::stoi(argv[2]);
	int num_consumers = std::stoi(argv[3]);
	int num_produce = std::stoi(argv[4]);
	
	//input validation check
	if (buffer_size <= 0 || num_producers <= 0 || num_consumers<= 0 || num_produce < 0){
		std::cerr << "Invalid paramaters. Buffer size, number of producers and number of consumers must be >=1; number to produce must be >=0.\n";
		return EXIT_FAILURE;
	}

	//display message for how many yodas will be produced
	std::cout << "Producing "<<num_produce<< " yodas today.\n";
	
	//Initialize bounded buffer
	BoundedBuffer buffer(buffer_size);
	
	//Create a thread pool with total number of threads
	ThreadPool threadPool(num_producers + num_consumers);

	//calculate items per producer
	int itemsPerProducer = num_produce / num_producers;
	int remaining_items = num_produce % num_producers;

	
	//Create and enqueue producer tasks
	for (int i = 0; i < num_producers; ++i){
		int produce_count = itemsPerProducer + (i < remaining_items ? 1 : 0);
		threadPool.enqueue([&buffer, i, produce_count]() {
			producer_task(produce_count, i+1, &buffer);
		});
	}
	
	//Create and enqueue consumer tasks
	for (int i = 0; i < num_consumers; ++i){
		threadPool.enqueue([i, &buffer](){
			consumer_task(i + 1, &buffer);
		});
	}
	
	//wait for all producers to finish producing
	{
		std::unique_lock<std::mutex> lock(cv_m);
		cv.wait(lock, [] {return doneProducing;});
	}

	//mark buffer as done
	buffer.setDone();

	//wait for all tasks to complete
	threadPool.waitForCompletion();

	//clean up the threadpool
	threadPool.threadpoolEnd();

	std::cout << "Producer/Consumer simulation complete!\n";
	return EXIT_SUCCESS;
}
