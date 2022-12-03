// Alwaysy centred around 0
class Vector {
  float x, y;
  
  Vector(float xInit, float yInit) {
    x = xInit;
    y = yInit;
  }
  
  float length() { 
    return sqrt(x * x + y * y); 
  }
  
}
