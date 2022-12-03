class Circle {
  float mass = 10;
  float xPos, yPos; 
  
  float xVel = 0.0;
  float xAccel = 0.0;
  
  float yVel = 0.0;
  float yAccel = 0.0;
  
  int diameter = 50;
  
  Circle(float x, float y) {
    xPos = x;
    yPos = y;
    
    // Draw the initia circle
    ellipseMode(CENTER);         // Draw a circle to begin with
    fill(255);
    update();
  }
  
  void calculatePosition() {
    yAccel = gravity;
    yVel += yAccel * SECONDS_PER_CYCLE;
    yPos += yVel * SECONDS_PER_CYCLE;
   
    
    // Close to ground
    if (yPos > height - diameter/2) {
      
      // Let it sit after a while
      if (abs(yVel) < 3)
      {
        yVel = 0.0;
      }
      // Return the bounce
      else
      {
        yAccel *= elasticity;
        yVel *= elasticity;
      }
      
    }
  }
  
  void update() {
    println(xPos + " : " + yPos);
    
    // Limiting with ground
    if (yPos > height - diameter/2) { yPos = height - diameter/2; }
    
    ellipse(xPos, yPos, diameter, diameter);
  }
  
  void resetMotion() {
    xVel = 0.0;
    xAccel = 0.0;
    
    yVel = 0.0;
    yAccel = 0.0;
  
  }
}
