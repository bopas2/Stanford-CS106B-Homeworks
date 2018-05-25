PVector location;
PVector velocity;
float gravity = -.5;
void setup() {
  size(500,500);
  smooth();
  background(255);
  location = new PVector(100,100);
  velocity = new PVector(2.5,5);
}
void draw() {
  velocity.y = velocity.y - gravity;
  location.add(velocity);
  clear();
  background(255);
  if(location.x <= 16 || location.x >= width - 16) {
    velocity.x *= -1;
  }
  if(location.y >= width - 16) {
    location.y = 483;
    velocity.y *= -1;
  }
  if(location.y <= 16) {
    location.y = 17;
    velocity.y *= -1;
  }
  fill(0);
  ellipse(location.x,location.y,32,32);
}