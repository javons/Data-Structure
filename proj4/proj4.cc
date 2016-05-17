/**
 * @file proj4.cc
 * \mainpage
 *
 * CISC 2200, Data Structures<br>
 * Project 4: The Banking Simulation<p>
 *
 * Queue-based simulation of a bank having only one teller. 
 * (This is Programming Problem 13.6 of our text.)
 *
 * @author Tianmao Jiang
 * @date   April 15
 *
 **********************************************************************/


#include <cstdlib>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <queue>

using namespace std;

/**Keep track of availability of teller*/
bool tellerAvailable;
/**initiate current time*/
int currentTime;
/**Keep track of number of customers*/
int customerNum;
/**Decide whether to print final statistics or not*/
int flag;

/**Only two kinds of events.*/
enum EventKind {arrival, departure};

/**
 * This holds the information needed for an arrival or departure event. 
 */
struct Event
{
/**What kind of event this is(arrival or departure)*/
    EventKind type;
/**Arrival or departure time of the event, as 
 *approoriate
 */
    int time;
/**Duration of the event.(A departure event has a duration of 0.)*/
    int length;
    
/**
 * Constructor, as one might expect.
 * @param theType what kind of event this is (arrival or departure)
 * @param theTime arrival or departure time of the event, as
 * appropriate
 * @param theLength duration of the event.
 */ 
    Event(EventKind theType=arrival, int theTime=0, int theLength=0):
	type(theType),
	time(theTime),
	length(theLength) {}
};

/** Shorthand for how we implement the EventList. */
using EventPQType = priority_queue<Event> ;

/** Shorthand for how we represent the customers who have queued up at
 * the bank
 */
using BankQType = queue<Event>;

/**
 * holds arrival and departure events that haven't yet arrived at the
 * bank
 */
struct EventList
{
/**priority queue that holds the event list itself*/
    EventPQType epq;
    
/** 
 * fill the event list from an input stream
 * @param is the input stream
 * @post the event list has been filled up
 */
    void fill(istream& is);
    
/**
 * actually does the bank simulation
 * @post all customers have been processed
 */  
    void simulate();
};

/**
 * Less-than operator for Events.
 * This is somewhat counter-intuitive, since the first element of a
 * priority_queue is the greatest element, rather than the smallest
 * element.
 * @param e0 an event
 * @param e1 an event
 * @return true iff the arrival time for e0 is later than that of e1
 */
bool operator<(const Event& e0, const Event& e1);

/**
 * Input stream extraction operator for Events.
 * @param is the input stream
 * @param e the Event to be extracted from is
 * @return the "new" is
 */
istream& operator>>(istream& is, Event& e);

/**
 * process an arrival event
 * @param e the event
 * @param epq the event list
 * @param bq the queue of customers who are actually in line
 * @param tellerAvailable is a teller available?
 */
void processArrival(const Event& e, EventPQType& epq, BankQType& bq,
		    bool& tellerAvailable);

/**
 * process a departure event
 * @param e the event
 * @param epq the event list
 * @param bq the queue of customers who are actually in line
 * @param tellerAvailable is a teller available?
 * @return the waiting time for this departure Event
 */
int processDeparture(const Event& e, EventPQType& epq, BankQType& bq,
		     bool& tellerAvailable);


/**
 * The usual main function
 */
int main(int argc, char** argv)
{
    EventList eventList;

    // command-line parameter munging   
    // also fills the event list with the input data
    char* progname = argv[0];            // simplify msgs a bit
    switch (argc) {
    case 1:
	eventList.fill(cin);
	break;
    case 2: {
	ifstream ifs(argv[1]);
	if (!ifs) {
	    cerr << progname << ": couldn't open " << argv[1] << endl;
	    return 1;
	}
	eventList.fill(ifs);
	break;
    }
    default:
	cerr << "Usage: " << progname << " [datafile]\n";
	return 2;
    }

    eventList.simulate();
}



bool operator<(const Event& e0, const Event& e1)
{
    if (e0.time > e1.time)
	return true;
    else if (e0.time == e1.time && e0.type == departure)
	return true;
    else
	return false;
}


istream& operator>>(istream &is, Event &e)
{
    is >> e.time;
    is >> e.length;
    return is;
}


void EventList::fill(istream& is)
{
    Event e;
    queue<Event> test;
    while(is>>e) {
	int newTime = e.time;
	int newLen = e.length;
	if ((!test.empty()) && (newTime < test.back().time)) {
	    flag = 1;
	    cout << "customer #" << customerNum + 1
		 << " out of order (time = " << newTime
		 << ", previous time = " << test.back().time << ")" << endl;
	    epq = priority_queue<Event>();
	    break;
	}
	test.push(Event{arrival, newTime, newLen});
	epq.push(Event{arrival, newTime, newLen});
	customerNum++;
    }
}


void EventList::simulate()
{
    int totalWait = 0;
    bool tellerAvailable = true;
    BankQType bq;
    
    while (!epq.empty())  
	{  
	    Event e = epq.top();  
	    currentTime = e.time;  
	    
	    if (e.type == arrival) {
		cout << "Processing an arrival event at time: "
		     << e.time << "\n";
		processArrival(e, epq, bq, tellerAvailable);
	    }
	    else {
		cout << "Processing a departure event at time: "
		     << e.time << "\n";
		totalWait += processDeparture(e, epq, bq, tellerAvailable);
	    }
	}
    if (!flag) {
	cout << "Final statistics:\n";
	cout << "Total number of people processed: " << customerNum << "\n"
	     << "Average amount of time spent on waiting: "
	     << (double)totalWait / customerNum << "\n";
    }
}

void processArrival(const Event& e, EventPQType& epq, BankQType& bq,
		    bool& tellerAvailable)
{
     
    epq.pop();
    if (bq.empty() && tellerAvailable)
	{  
	    int departureTime = e.time + e.length;
	    Event nDE{departure, departureTime, 0};  
	    epq.push(nDE);
	    tellerAvailable = false;
	}  
    else
	{
	    bq.push(e);
	}
}


int processDeparture(const Event& e, EventPQType& epq, BankQType& bq,
		     bool& tellerAvailable)
{
    int wait = 0;
    epq.pop();  
    if (!bq.empty())  
	{  
	    Event nowCustomer = bq.front();  
	    int departureTime = e.time + nowCustomer.length;
	    bq.pop();
	    Event nDE{departure, departureTime, 0};  
	    epq.push(nDE);  
	    wait = e.time - nowCustomer.time;  
	}  
    else  
	{
	    tellerAvailable = true;  
	}
    return wait;
}
