// The screen will turn a different colour when the ball is within the bound of the circle
//   + Will also be able to be moved across the screen on click
// 21/11/2018
// Lyall Beveridge

int circleSize = 50;           // The dimensions of the circle
int circleX, circleY;          // Hold the variables for the circle position

boolean draggable;             // Allows the circle to be dragged

void setup() {
  size(600, 600);              // Draw a square background
  
  circleX = width/2;
  circleY = height/2;
  
  // Initialise the draggable features to false
  draggable = false;
  
  background(255, 0, 0);       // Set the background red
  
  ellipseMode(CENTER);         // Draw a circle to begin with
  fill(255);
  ellipse(circleX, circleY, circleSize, circleSize);
}


void draw() {
  // Do nothing
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
    background(0, 255, 0);      // Green background    
  } else {
    draggable = false;
    background(255, 0, 0);      // Red background
  }
  
  ellipse(circleX, circleY, circleSize, circleSize);
}


// If the mouse is being dragged where the beginning click started over the mouse, move the circle
void mouseDragged() {
  println("user dragging; draggable =", draggable);
  if(draggable) {
    // Set the circle to the mouse pointer
    ellipse(mouseX, mouseY, circleSize, circleSize);
  }
}
