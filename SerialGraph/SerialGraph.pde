import processing.serial.*;

Serial bt;

int nextDataIndex = 0;
int[][] data;
int seriesCount = 3;

void setup () {
  // set the window size:
  size(1400, 800);
  frameRate(60);
  smooth();
  
  data = new int[seriesCount][width];

  bt = new Serial(this, "/dev/tty.HC-06-DevB", 9600);
  // don't generate a serialEvent() unless you get a newline character:
  bt.bufferUntil('\n');
}

void draw () {
  background(0);
  for (int x = 0; x < width - 1; x++) {
    for (int i = 0; i < seriesCount; i++) {
      stroke(
          i == 0 ?  0 : 255,
          i == 1 ?  0 : 255,
          i == 2 ?  0 : 255);
      line(x, data[i][x], x+1, data[i][x + 1]);
    }
  }
  noLoop();
}

void mouseClicked() {
  bt.write("DiagCollect\n");
}

void serialEvent (Serial bt) {
  // get the ASCII string:
  String input = bt.readStringUntil('\n');

  if (input == null) 
    return;

  String[] parts = split(input, ',');

  if (parts.length != seriesCount)
    return;
  // convert to an int and map to the screen height:
  float value;
  for (int i = 0; i < seriesCount; i++) {
    value = float(parts[i]); 
    value = map(value, -180, 180, 0, height);
    data[i][nextDataIndex] = int(value);
  }
  ++nextDataIndex;
  nextDataIndex %= width;
  loop();
}

