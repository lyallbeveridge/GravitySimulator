// The ball can be dragged and dropped
// + The velocity will be based off of a constant value
// 21/11/2018
// Lyall Beveridge

import hypermedia.net.*;       // UDP Library?
UDP udp;

int fallDist;                  // The height between the ball and the ground

int circleSize = 50;           // The dimensions of the circle
int circleX, circleY;          // Hold the variables for the circle position

boolean draggable;             // Allows the circle to be dragged

int velocity;                  // Velocity of the ball
int accelerationConst = 2;     // Acceleration constant

float elasticityConst = -0.9;  // The elasticity constant determines the amount with which the ball bounces back

int grounded;                  // Record the Y value at which the ball is considered on 'on the ground'


void setup() {
  size(900, 900);              // Draw a square background
  
  velocity = 0;                // Iinitialise the movement variables
  grounded = height - (circleSize/2);  
  draggable = false;           // Initialise the draggable features to false
  
  // Drawing
  circleX = width/2;
  circleY = height/2;
  background(0, 255, 0);       // Green background 
  ellipseMode(CENTER);         // Draw a circle to begin with
  fill(255);
  ellipse(circleX, circleY, circleSize, circleSize);
 
  // UDP Setup
  udp = new UDP(this, 6000);
  udp.listen( true );
  
  
  // Let the user know that it's working
  println("The program has started");
 
}


// This function will receive the UDP datagrams
void receive( byte[] data, String ip, int port ) {  // <-- extended handler
  
  
  // get the "real" message =
  // forget the ";\n" at the end <-- !!! only for a communication with Pd !!!
  data = subset(data, 0, data.length-2);
  String message = new String( data );
  
  // print the result
  println( "receive: \""+message+"\" from "+ip+" on port "+port );
}


void draw() {
  // If the ball being clicked or dragged (i.e. the mouse isn't pressed whilst the cursor is on the ball)
  if ((!draggable || mouseButton != LEFT) && circleY != grounded) acceleration();
  
  // Drawing code
  background(0, 255, 0);      // Green background 
  ellipse(circleX, circleY, circleSize, circleSize);
}


// If the mouse moves within the bounds of the circle set the background of
// the screen to green
// Locals:
float dist;                      // Distance between mouse
void mouseMoved(){
  // Checks whether the distance between the mouse and the point is below threshold
  dist = dist(mouseX, mouseY, circleX, circleY);
  if (dist < circleSize) {
    draggable = true;   
  } else {
    draggable = false;
  }
}


// If the mouse is being dragged where the beginning click started over the mouse, move the circle
void mouseDragged() {
  if(draggable) {
    
    // Set the circle to the mouse pointer
    circleX = mouseX;
    circleY = mouseY;
    redraw();
  }
}


// This function will determine the new velocity value according to the location of the ball
void acceleration() { 
    // Delay the falling and print those values
    // println("Values: velocity", velocity, "accelerationConst", accelerationConst);
    
    fallDist = height - circleY;
    
    // Call the acceleration function to determine the velocity
    velocity += accelerationConst;           // Calculate the velocity based on the last 
    
    // Make the ball fall with a constant velocity towards the ground
    if (fallDist <= velocity + (circleSize/2)) {
      circleY = grounded;                    // Then move it to 0
      velocity *= elasticityConst;           // Reset the velocity to 0
      // Determine whether to bounce or not
    }
    circleY += velocity;                     // Move downwards
}
