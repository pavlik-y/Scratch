class PidController {
  public PidController() {
    ie = 0.0;
    lambda = 0.1;
    Kp = 0.3;
    Ki = 0.3;
    Kd = 0.3;
    setpoint = 0.0;
  }
  
  void CalcOutput(float v, float de) {
    float e = v - setpoint;
    ie += lambda * e;
    output = Kp * e + Ki * ie + Kd * de;
  }
  
  void SetCoefficients(float Kp_, float Ki_, float Kd_) {
    Kp = Kp_;
    Ki = Ki_;
    Kd = Kd_;
  }
  
  void SetLambda(float lambda_) {
    lambda = lambda_;
  }
  
  void SetSetpoint(float setpoint_) {
    setpoint = setpoint_;
  }
  
  float output;
  float ie;
  float lambda;
  float Kp;
  float Ki;
  float Kd;
  float setpoint;
}

class PhysicalModel {
  public PhysicalModel() {
    speed = 0;
  }
  void MeasureAndUpdate(float output) {
    last_speed = speed;
    speed += ((output / 256.0) * 75.0 - speed) * 0.1;
    acceleration = (speed - last_speed) * 100.0; 
  }
  
  float speed;
  float acceleration;
  float last_speed;
}

float[] speed_data;
float[] output_data;
float[] acceleration_data;

PidController pid_controller;
PhysicalModel physical_model;

void PopulateData() {
  speed_data = new float[width];
  output_data = new float[width];
  acceleration_data = new float[width];

  pid_controller = new PidController();
  pid_controller.SetLambda(0.01);
  pid_controller.SetCoefficients(-20, -50, 0);
  pid_controller.SetSetpoint(25);
  physical_model = new PhysicalModel();
  
  float output = 0;
  for (int i = 0; i < speed_data.length; i++) {
    physical_model.MeasureAndUpdate(output);
    output_data[i] = output;
    speed_data[i] = physical_model.speed;
    acceleration_data[i] = physical_model.acceleration;
    pid_controller.CalcOutput(speed_data[i], acceleration_data[i]);
//    output = constrain(output + pid_controller.output, -255.0, 255.0);
    output = constrain(pid_controller.output, -255.0, 255.0);
    println(output_data[i], speed_data[i], acceleration_data[i]);
  }
}

void setup () {
  // set the window size:
  size(1400, 800);
  smooth();
  
  PopulateData();
}

float drawMap(float value, float scale) {
  return map((float)value, -scale, scale, (float)height, 0.0); 
}

float speed_scale = 50.0;
float output_scale = 260.0;
float acceleration_scale = 100.0;

void draw () {
  background(0);
  stroke(64, 64, 64);
  for (int x = 0; x < width; x += 100) {
    line(x, 0, x, height);
  }
  
  for (int y = 0; y < height; y += 200) {
    line(0, y, width, y);
  }
  stroke(0, 255, 255);
  for (int x = 0; x < width - 1; x++) {
    line(x, drawMap(speed_data[x], speed_scale), x+1, drawMap(speed_data[x + 1], speed_scale));
  }
  stroke(255, 0, 255);
  for (int x = 0; x < width - 1; x++) {
    line(x, drawMap(output_data[x], output_scale), x+1, drawMap(output_data[x + 1], output_scale));
  }
  stroke(255, 255, 0);
  for (int x = 0; x < width - 1; x++) {
    line(x, drawMap(acceleration_data[x], acceleration_scale), x+1, drawMap(acceleration_data[x + 1], acceleration_scale));
  }
  noLoop();
}

void mouseClicked() {
}


