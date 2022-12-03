// The ball can be controlled by a controller
//     - Positive is down
//     - Negative is up
//
// 3/12/2022
// Lyall Beveridge

final int SECONDS_PER_CYCLE = 1;

import hypermedia.net.*;       // UDP Library required import
UDP udp;

int gravity = 2;               // Acceleration constant
float elasticity = -0.9;       // The elasticity constant determines the amount with which the ball bounces back

Circle circle;


void setup() {
  size(900, 900);              // Draw a square background
  background(0, 255, 0);       // Green background 
  
  // Circle declaration
  circle = new Circle(height/2, width/2);
 
  // UDP Setup
  udp = new UDP(this, 6000);
  udp.listen( true );
  
  println("The program has started");   // Let the user know that it's working
}


// This function will receive the UDP datagrams
void receive( byte[] data, String ip, int port ) {  // <-- extended handler
  data = subset(data, 0, data.length-2);
  String message = new String( data );
  //println( "receive: \""+message+"\" from "+ip+" on port "+port );

  // Switch depending on the buttons
  switch(message) {  
    case "2": 
      circle.yVel -= 20;
      println("Received upwards command!");
      break;
    default:
      break;
  }
}


void draw() {
  // If the mouse is clicked put the ball there
  if (mousePressed == true) 
  {
    circle.xPos = mouseX;
    circle.yPos = mouseY;
    circle.resetMotion();
  }
  // If the mouse is not clicked let the ball continue
  else
  {
    circle.calculatePosition();
  }

  // Green background and draw the Circle
  background(0, 255, 0);
  circle.update();
}
