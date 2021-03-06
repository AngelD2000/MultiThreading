# Multi-Threaded Programming 
In this assignment develop a multi-threaded application, written in C, that resolves domain names to IP addresses.  This is similar to the operation performed each time you access a new website in your web browser.  The application will utilize two types of worker threads: requesters and resolvers.  These threads communicate with each other using a shared array, also referred to as the bounded buffer. 

### Requester Threads
The application will take an argument for the number of requester threads.  This pool of threads service a set of text files as input, each of which contains a list of hostnames.  Each name read should be placed on the shared array.  If a requester thread tries to write to the array but finds that it is full, it should block until a space opens up in the array. 

### Resolver Threads
The second thread pool is comprised of a set of resolver threads.  A resolver thread consumes the shared array by taking a name off the array and resolving its IP address. After the name has been mapped to an IP address, the output is written to a line in the resolver logfile. If the resolver is unable to find the IP address for a hostname, it should leave NOT_RESOLVED. If a resolver thread tries to read from the array but finds that it is empty, it should block until there is a new item in the array or all names in the input files have been serviced.

### Shared Array 
For this project I implemented a circular queue as the shared array and used mutex locks in <pthread.h> and semaphores in <semaphores.h> to ensure thread safe code in this program. Circular queue preserves some level of ordering for execution and I didn't need to worry about moving indexes since it's a circular queue. The mutex lock is created in the header file and initialized when main thread calls to create the queue. There wasn't a need to implement isFull and isEmpty to check in enqueue and dequeue functions because the thread will just be blocked by the semaphores if the queue is empty in dequeue and if the queue is full in enqueue. Everytime an item successfully inputed into the queue, the semaphore that keeps track of spaces available decrements by one and item avaliable increments by one. Everytime an item successfully dequeues from the queue, the semaphore that keeps track of items available decrements by one and space avaliable increments by one. 

## Things I learned:
1. Writing thread safe code is difficult.... When reading and writing to and from the same buffer even if it's two different functions need to use the same mutex locks. Otherwise even though it made producer threads thread safe from one another it did not make producer and consumer threads thread safe from one another. 
2. Memory leaks should be constantly checked. It might have worked with one version of the code but may fail when something small is changed. All memories should be freed and always double check if it's freeing something  that the memory is not allocated for. 
3. Always test for edge cases even when you think you've tested for them all. It's always easier to see the program fail than to try to reason through if it is going to fail. fclose(NULL) apparently just segmentation faults instead of an error. 
4. Even though the program may still not work (I'm not sure if it 100% works even now) it's ok, just keep testing until you have to turn it in. 
