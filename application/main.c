#include <iostream>
#include "Leap.h"
#include "ControllerListner.h"

using namespace std;


int main() {
    // Create a listener and controller
    ControllerListener listener;
    Controller controller;
    
    // Create serial connector
    //SerialConnector
    listener.connector.ConnectionOpen("/\dev/\cu.usbmodemfd1221", 9600);
    
    //listener.connector.send("qweqwe");
    
   // listener.addConnector(connector);
    
    // Have the listener receive events from the controller
    // and start loop - onFrame
    
    //-----
    controller.addListener(listener);
    
    
    // Keep this process running until Enter is pressed
    cout << "Press Enter to quit..." << endl;
    cin.get();
    
    // Remove the sample listener when done
    controller.removeListener(listener);
    
    return 0;
}
