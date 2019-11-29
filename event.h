#ifndef         _EVENT_H  
#define         _EVENT_H

#define         INVALID_TYPE    (-1)
#define         INVALID_TIME    (-1.0)

#include <iostream>
using namespace std;

#include        <stdlib.h>
#include        "mydef.h"
#include        "UE.h"


class Event {
 private:
    int                eventType;    /* eventType=1 for ARRIVAL,=2 for CREDIT_RESERVATION ,=3 for CREDIT_CONSUMED , =4 RANDOM_CHECK*/
    double              timeStamp;      /* event time stamp             */
    double              startTime ;
    double              delayTime ;
    int                arrivalType ;
    Event*             next;           /* pointer to next event node   */
    static Event*      freeList;       /* pointer to free event list   */
    int                timeslot;
    int                delayTimeIndex ;
    int                DRBid; //Wifi_sim.cpp用
    UE*                drb; //equalSharing.cpp用
    int                 deviceType; //0:AP 1:eNB
    int                 ap_id;//APfirst.cpp用
    int                 eNB_id;//APfirst.cpp用

    
    
 public:
        Event() { eventType=0; /* constructor  */
                  timeStamp=0;
                  arrivalType = 0 ;
                  next      = NULL;
                }
        Event(const int,const double ,const int);
                   
        void    initNode( const int,
                          const double, /* event node initialization    */
                          const int
                         ); /*  setup timestamp,old     */
        void    insNext( Event*);       /* insert the event node next   */
                                        /*  to the current node         */
        void    delNext();              /* delete the event node next   */
                                        /*  to the current node         */
        Event*  getNext()               /* get the pointer to the next  */
                { return next; }        /*  event node                  */
        double  getTimeStamp()      /* get the timestamp of current node*/
                { return timeStamp; }
       
        int     getEventType() {return eventType;}
        int     getArrivalType(){return arrivalType;}
        
        void  setEventType(int t) {eventType=t; }
        void  setarrivalType(int s){arrivalType=s;}    /* set arrivalType      */
        void  setTimestamp(double t)        /* set timestamp             */
              { timeStamp=t;}
        void  setNext(Event *E) { next=E;}
        void*   operator new(size_t);   /* new operator of class Event  */
        void    operator delete( void*, /* delete operator of class     */
                                 size_t);/* Event                       */
        friend ostream& operator<<( ostream&,   /* print the content    */
                                    Event&);    /*  of the event node   */
    
    void setTimeSlot(int t){timeslot = t ;}
    int getTimeSlot(){return timeslot;}
    
    void setStartTime(double t){startTime = t ;}
    double getStartTime(){return startTime;}
    
    void setDelayTime(double t){delayTime = t ;}
    double getDelayTime(){return delayTime;}
    
    void setCurrentDelayTimeIndex(int i){ delayTimeIndex = i ;}
    int getCurrentDelayTimeIndex(){return delayTimeIndex;}

    void setDRBid(int i){DRBid = i;}
    int getDRBid(){return DRBid;}

    void setDRB(UE* in){drb = in;}
    UE* getDRB(){return drb;}

    void setDeviceType(int d_type){deviceType = d_type;}
    int getDeviceType(){return deviceType;}

    void setApId(int id){ap_id = id;}
    int getApId(){return ap_id;}
    
    void setENBid(int id){eNB_id = id;}
    int getENBid(){return eNB_id;}

    ~Event() { delete next;}
           /*        destructor                   */ 
};
                
#endif

