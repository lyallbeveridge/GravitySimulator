// The ball can be dragged and dropped
// + The velocity will be based off of a constant value
// 21/11/2018
// Lyall Beveridge

int fallDist;                  // The height between the ball and the ground

int velocityConst = 8;         // The ball will move downwards at a constant number of pixels

int circleSize = 50;           // The dimensions of the circle
int circleX, circleY;          // Hold the variables for the circle position

boolean draggable;             // Allows the circle to be dragged

void setup() {
  size(600, 600);              // Draw a square background
  
  circleX = width/2;
  circleY = height/2;
  
  // Initialise the draggable features to false
  draggable = false;
  
  background(0, 255, 0);       // Green background 
  
  ellipseMode(CENTER);         // Draw a circle to begin with
  fill(255);
  ellipse(circleX, circleY, circleSize, circleSize);
}


void draw() {
  // If the ball being clicked or dragged (i.e. the mouse isn't pressed whilst the cursor is on the ball)
  if (!draggable || mouseButton != LEFT ) {
    fallDist = height - circleY;
    // Make the ball fall with a constant velocity towards the ground
    if (fallDist == circleSize/2) {
      ;                 // Save time by breaking out of the loop 
    } else if (fallDist < velocityConst + (circleSize/2)) {
      circleY = height - (circleSize/2);      // Then move it to 0
    } else {
      circleY += velocityConst;          // Move downwards
    }
  }
  
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
