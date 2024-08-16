# AFIT-CSCE489-PROJ2

# Overview
The files contained in this project create a simulation for a single-producer, multiple-consumer storefront. When launching the simulation from a terminal use './babyyoda <buffer_size> <num_consumers> <max_items>'. This is will begin the simulation based off of your desired inputs.  If you input invalid values or do not enter enough arguments the program will notify you to try again and tell you what you did wrong.

# babyyoda.cpp
babyyoda.cpp contains the main method for the simulation as well as the producer and consumer routines that are used by the pthreads the main method creates.

# Bounded_Buffer.cpp
Bounded_Buffer.cpp contains the methods necessary to impliment a bounded buffer to be used by the storefront.  It also contains the constructor for the Item class which is used to define the items (yoda) that are used by the simulation.

# Bounded_Buffer.h
Bounded_Buffer.h contains the necessary headers for the functions used by Bounded_Buffer.cpp.  It also contains the classes for Item and BoundedBuffer.

# Semaphore.cpp
Semaphore.cpp contains the methods necessary for the simulation to utilize semaphores with mutex locks

# Semaphore.h
Semaphore.h contains the necessary headers and the Semaphore class for Semaphore.cpp.

# MakeFile
The makefile for this package has been adjusted to compile with all of my code. It is important that you use my makefile or the code will not compile correctly.
