#include <iostream>
#include "Leap.h"
#include "SerialConnector.h"

using namespace Leap;

class ControllerListener : public Listener {
public:
    SerialConnector connector;
    virtual void onInit(const Controller&);
    virtual void onConnect(const Controller&);
    virtual void onDisconnect(const Controller&);
    virtual void onExit(const Controller&);
    virtual void onFrame(const Controller&);
    virtual void onFocusGained(const Controller&);
    virtual void onFocusLost(const Controller&);
};

void ControllerListener::onInit(const Controller& controller) {
    std::cout << "Initialized" << std::endl;
}

void ControllerListener::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
    controller.enableGesture(Gesture::TYPE_CIRCLE);
    controller.enableGesture(Gesture::TYPE_KEY_TAP);
    controller.enableGesture(Gesture::TYPE_SCREEN_TAP);
    controller.enableGesture(Gesture::TYPE_SWIPE);
    
}

void ControllerListener::onDisconnect(const Controller& controller) {
    //Note: not dispatched when running in a debugger.
    std::cout << "Disconnected" << std::endl;
}

void ControllerListener::onExit(const Controller& controller) {
    std::cout << "Exited" << std::endl;
}


void ControllerListener::onFrame(const Controller& controller) {
    // Get the most recent frame and report some basic information
    const Frame frame = controller.frame();
    std::cout << "Frame id: " << frame.id()
    << ", timestamp: " << frame.timestamp()
    << ", hands: " << frame.hands().count()
    << ", fingers: " << frame.fingers().count()
    << ", tools: " << frame.tools().count()
    << ", gestures: " << frame.gestures().count() << std::endl;
    
    
    char buf[250];
    if(frame.fingers().count() <= 1){
        connector.send("[e]");
        return;

    }
    sprintf(buf, "[f%d]", frame.fingers().count());
    connector.send(buf);
    
    
    if (!frame.hands().isEmpty()) {
        // Just track only one hand
            const Hand hand = frame.hands()[0];
            
        
            Vector palmPosition = hand.palmPosition();
            sprintf(buf, "[x%d][y%d][z%d]", (int)palmPosition.x, (int)palmPosition.y, (int)palmPosition.z);
            connector.send(buf);
            
            // Check if the hand has any fingers
            const FingerList fingers = hand.fingers();
            if (!fingers.isEmpty()) {
                // Calculate the hand's average finger tip position
                Vector avgPos;
                for (int i = 0; i < fingers.count(); ++i) {
                    avgPos = fingers[i].tipPosition();
                    std::cout  << " Finger tip position" << avgPos << std::endl;
                }
                /*
                 avgPos /= (float)fingers.count();
                 std::cout << "Hand has " << fingers.count()
                 << " fingers, average finger tip position" << avgPos << std::endl;
                 */
            }
            // Get the hand's sphere radius and palm position
            std::cout << "Hand sphere radius: " << hand.sphereRadius()
            << " mm, palm position: " << hand.palmPosition() << std::endl;
            // Get the hand's normal vector and direction
            const Vector normal = hand.palmNormal();
            const Vector direction = hand.direction();
            // Calculate the hand's pitch, roll, and yaw angles
            std::cout << "Hand pitch: " << direction.pitch() * RAD_TO_DEG << " degrees, "
            << "roll: " << normal.roll() * RAD_TO_DEG << " degrees, "
            << "yaw: " << direction.yaw() * RAD_TO_DEG << " degrees" << std::endl;
        
        
        
        
        
    }
    
    // Get gestures
    const GestureList gestures = frame.gestures();
    for (int g = 0; g < gestures.count(); ++g) {
        Gesture gesture = gestures[g];
        
        switch (gesture.type()) {
            case Gesture::TYPE_CIRCLE:
            {
                CircleGesture circle = gesture;
                std::string clockwiseness;
                
                if (circle.pointable().direction().angleTo(circle.normal()) <= PI/4) {
                    clockwiseness = "clockwise";
                } else {
                    clockwiseness = "counterclockwise";
                }
                
                // Calculate angle swept since last frame
                float sweptAngle = 0;
                if (circle.state() != Gesture::STATE_START) {
                    CircleGesture previousUpdate = CircleGesture(controller.frame(1).gesture(circle.id()));
                    sweptAngle = (circle.progress() - previousUpdate.progress()) * 2 * PI;
                }
                std::cout << "Circle id: " << gesture.id()
                << ", state: " << gesture.state()
                << ", progress: " << circle.progress()
                << ", radius: " << circle.radius()
                << ", angle " << sweptAngle * RAD_TO_DEG
                <<  ", " << clockwiseness << std::endl;
                break;
            }
            case Gesture::TYPE_SWIPE:
            {
                SwipeGesture swipe = gesture;
                std::cout << "Swipe id: " << gesture.id()
                << ", state: " << gesture.state()
                << ", direction: " << swipe.direction()
                << ", speed: " << swipe.speed() << std::endl;
                break;
            }
            case Gesture::TYPE_KEY_TAP:
            {
                KeyTapGesture tap = gesture;
                std::cout << "Key Tap id: " << gesture.id()
                << ", state: " << gesture.state()
                << ", position: " << tap.position()
                << ", direction: " << tap.direction()<< std::endl;
                break;
            }
            case Gesture::TYPE_SCREEN_TAP:
            {
                ScreenTapGesture screentap = gesture;
                std::cout << "Screen Tap id: " << gesture.id()
                << ", state: " << gesture.state()
                << ", position: " << screentap.position()
                << ", direction: " << screentap.direction()<< std::endl;
                break;
            }
            default:
                std::cout << "Unknown gesture type." << std::endl;
                break;
        }
    }
    
    if (!frame.hands().isEmpty() || !gestures.isEmpty()) {
        std::cout << std::endl;
    }
}

void ControllerListener::onFocusGained(const Controller& controller) {
    std::cout << "Focus Gained" << std::endl;
}

void ControllerListener::onFocusLost(const Controller& controller) {
    std::cout << "Focus Lost" << std::endl;
}