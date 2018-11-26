// The screen will turn a different colour when the ball is within the bound of the circle
// 21/11/2018
// Lyall Beveridge

int circleSize = 50;           // The dimensions of the circle
int circleX, circleY;          // Hold the variables for the circle position

void setup() {
  size(600, 600);              // Draw a square background
  
  circleX = width/2;
  circleY = height/2;
  
  background(255, 0, 0);       // Set the background red
  
  ellipseMode(CENTER);         // Draw a circle to begin with
  fill(255);
  ellipse(circleX, circleY, circleSize, circleSize);
}

void draw() {
  // If this doesnt draw anything then maybe the program will work
}

// If the mouse moves within the bounds of the circle set the background of
// the screen to green
// Locals:
float dist;                      // Distance between mouse

void mouseMoved(){
  // Checks whether the distance between the mouse and the point is below threshold
  dist = dist(mouseX, mouseY, circleX, circleY);
  println("dist: ", dist);      // Print the circle values 
  if (dist < circleSize) {
    background(0, 255, 0);      // Green background
  } else {
    background(255, 0, 0);      // Red background
  }
  
  // Redraw the circle as the mouse moves
  ellipse(width/2, height/2, circleSize, circleSize);
  
}
