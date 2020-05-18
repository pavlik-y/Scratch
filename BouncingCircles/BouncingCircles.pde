class Circle {
  public float radius;
  public float x;
  public float y;
  public float dx;
  public float dy;
  public boolean solid;
  public Circle(float radius, float x, float y, float dx, float dy) {
    this.radius = radius;
    this.x = x;
    this.y = y;
    this.dx = dx;
    this.dy = dy;
  }
}

ArrayList<Circle> circles;

void SetupCircles() {
  circles = new ArrayList();
  for (int i = 0; i < 50; i++) {
    float radius = 10 + abs(randomGaussian()) * 10;
    float x = radius + random(width - radius * 2);
    float y = radius + random(height - radius * 2);
    float vx = randomGaussian() * 100;
    float vy = randomGaussian() * 100;
    Circle c = new Circle(radius, x, y, vx, vy);
    circles.add(c);
  }
}

int lastTimestamp;

void setup() {
  // fullScreen();
  size(800, 500);
  ellipseMode(RADIUS);
  lastTimestamp = millis();
  SetupCircles();
  textAlign(LEFT, TOP);
  textSize(20);
}

void UpdatePositions(ArrayList<Circle> circles, float timeDelta) {
  for (Circle c : circles) {
    c.x = c.x + timeDelta * c.dx;
    c.y = c.y + timeDelta * c.dy;
    while (true) {
      boolean adjusted = false;
      if (c.x < c.radius) {
        c.x = c.radius * 2 - c.x;
        c.dx = -c.dx;
        adjusted = true;
      }
      if (c.x > width - c.radius) {
        c.x = 2 * (width - c.radius) - c.x;
        c.dx = -c.dx;
        adjusted = true;
      }
      if (c.y < c.radius) {
        c.y = c.radius * 2 - c.y;
        c.dy = -c.dy;
        adjusted = true;
      }
      if (c.y > height - c.radius) {
        c.y = 2 * (height - c.radius) - c.y;
        c.dy = -c.dy;
        adjusted = true;
      }
      if (!adjusted)
        break;
    }
  }
}

boolean CirclesIntersect(Circle c1, Circle c2) {
  float dx = c1.x - c2.x;
  float rSum = c1.radius + c2.radius;
  // if (abs(dx) > rSum)
  //   return false;
  float dy = c1.y - c2.y;
  if ((dx*dx + dy*dy) < rSum*rSum)
    return true;
  return false;
}

void UpdateCircleColor(ArrayList<Circle> circles) {
  for (int i = 0; i < circles.size(); i++)
    circles.get(i).solid = false;
  for (int i = 0; i < circles.size() - 1; i++) {
    for (int j = i + 1; j < circles.size(); j++) {
      if (CirclesIntersect(circles.get(i), circles.get(j))) {
        circles.get(i).solid = true;
        circles.get(j).solid = true;
      }
    }
  }
}

void draw() {
  clear();
  background(255);
  int timestamp = millis();
  if (timestamp - lastTimestamp < 10) return;
  float delta = float(timestamp - lastTimestamp) / 1000.0;
  lastTimestamp = timestamp;
  UpdatePositions(circles, delta);
  UpdateCircleColor(circles);
  for (Circle c : circles) {
    if (c.solid) {
      fill(255, 0, 0);
    } else {
      fill(0, 0, 255);
    }
    circle(c.x, c.y, c.radius);
    float scale = 0.3;
    float vx = c.x + c.dx * scale;
    float vy = c.y + c.dy * scale;
    line(c.x, c.y, vx, vy);
  }
  String fr = String.format("%2.1f", frameRate);
  text(fr, 0, 0);
}

boolean loopRunning = true;
void keyPressed() {
  if (loopRunning) {
    noLoop();
    loopRunning = false;
  } else {
    loop();
    loopRunning = true;
  }
}