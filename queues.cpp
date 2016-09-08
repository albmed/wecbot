#include <iostream>       // std::cin, std::cout
#include <queue>          // std::queue
#include <string>
#include <boost/thread.hpp>
#include <boost/thread/scoped_thread.hpp>
#include <boost/chrono.hpp>
#include <boost/atomic.hpp>

std::queue<std::string> myqueue;
boost::atomic<bool> running; 
boost::mutex m_queueMutex;

// To compile: g++ -g -o queues queues.cpp  -lboost_thread -lboost_system -lboost_chrono 

void push(std::string& val) { 
	
	boost::mutex::scoped_lock lock(m_queueMutex);
	myqueue.push(val); 
}

std::string pop() { 
	std::string val = ""; 

	boost::mutex::scoped_lock lock(m_queueMutex);
	
	if (!myqueue.empty()){ 	
		val = myqueue.front(); 
		myqueue.pop();
	}
		
	return val; 
}


void wait(int seconds) {
	boost::this_thread::sleep_for(boost::chrono::seconds(seconds));
}

void consume () { 
	while (running || !myqueue.empty()) { 
		if (!myqueue.empty()) { 
			std::string dt = pop();
			
			wait(10); 
			std::cout << "[CONS] -- Got: " << dt << std::endl; 
		}
		wait (1); 
	}
	
	std::cout << "[CONS] -- Quiting.. " << std::endl; 
}

void produce() {
	std::string data; 
		
	do { 
		std::cout << "# "; 
		std::cin >> data; 
		if (data != "0") {
			push(data); 
			std::cout << "[PROD] -- Add: " << data << std::endl; 
		}
	} while (data != "0"); 
	
	std::cout << "[PROD] -- Quiting.. " << std::endl; 
	
	running = false; 
}

int main() {
	std::cout << "Started... " << std::endl;

	running = true; 
	
	boost::thread producer = boost::thread(produce);
	boost::thread consumer = boost::thread(consume); 
	//boost::scoped_thread<> producer((boost::thread(produce)));
	//boost::scoped_thread<> consumer((boost::thread(consume)));

	producer.join(); 
	consumer.join(); 

	std::cout << "Ended... " << std::endl;
}

