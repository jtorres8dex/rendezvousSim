Table table;
int currentIndex = 0;
float robotSize = 20; // Size of the robot for visualization
ArrayList<PVector> waypoints = new ArrayList<PVector>();
HashMap<Integer, ArrayList<VehicleState>> timestepData = new HashMap<Integer, ArrayList<VehicleState>>();
int maxTimestep = 0;

class VehicleState {
  int id;
  float x, y, theta;
  
  VehicleState(int id, float x, float y, float theta) {
    this.id = id;
    this.x = x;
    this.y = y;
    this.theta = theta;
  }
}

void setup() {
  size(800, 800); // Set the size of the window
  frameRate(30); // Set the frame rate to 30 frames per second
  table = loadTable("../../logs/SIMULATION_TEST.csv", "header"); // Load the CSV file

  // Print the number of rows in the table for debugging
  println("Total rows in table: " + table.getRowCount());

  // Read the table and store waypoints
  for (TableRow row : table.rows()) {
    int logType = row.getInt(1); // Assuming the second column is logType
    if (logType == 1) { // Waypoint log type
      float x = row.getFloat(2); // Scaling for visualization
      float y = row.getFloat(3);
      waypoints.add(new PVector(x, y)); // Store waypoint
      println("Added waypoint at (" + x + ", " + y + ")"); // Debugging
    } else if (logType == 0) { // Vehicle state log type
      int t = row.getInt(0); // First column is timestep
      int id = row.getInt(2); // Third column is vehicle ID
      float x = row.getFloat(3); // Fourth column is x
      float y = row.getFloat(4); // Fifth column is y
      float theta = radians(row.getFloat(5)); // Sixth column is theta

      if (!timestepData.containsKey(t)) {
        timestepData.put(t, new ArrayList<VehicleState>());
        if (t > maxTimestep) {
          maxTimestep = t;
        }
      }
      timestepData.get(t).add(new VehicleState(id, x, y, theta));
    }
  }
}

void draw() {
  background(255); // Clear the background each frame

  // Translate origin to the center of the window
  translate(width / 2, height / 2);
  
  // Flip the y-axis to make positive y go up
  scale(1, -1);

  // Draw all waypoints at every frame
  drawWaypoints();

  // Draw the robot if there are more rows to process
  if (currentIndex <= maxTimestep) {
    if (timestepData.containsKey(currentIndex)) {
      ArrayList<VehicleState> states = timestepData.get(currentIndex);

      for (VehicleState state : states) {
        drawRobot(state.x, state.y, state.theta);
        println("Drawing vehicle " + state.id + " at (" + state.x + ", " + state.y + ") with theta: " + state.theta); // Debugging
      }
    }

    // Increment the index to read the next row in the next frame
    currentIndex++;
  } else {
    println("All rows processed."); // Debugging
  }
}

void drawWaypoints() {
  fill(0, 255, 255); // Set fill color for waypoints
  
  for (PVector waypoint : waypoints) {
    ellipse(waypoint.x, waypoint.y, 10, 10); // Draw waypoint as a circle
    println(waypoints.size());
  }
}

void drawRobot(float x, float y, float theta) {
  println("Drawing robot at (" + x + ", " + y + ") with theta: " + theta); // Debugging
  pushMatrix(); // Save the current transformation matrix
  translate(x, y); // Move to the robot's position
  float theta_deg = degrees(theta);
  rotate(theta_deg); // Rotate to the robot's orientation
  fill(100, 100, 250); // Set the fill color for the robot
  rectMode(CENTER); // Draw the rectangle from its center
  rect(0, 0, robotSize, robotSize); // Draw the robot as a rectangle
  strokeWeight(6);
  stroke(0, 0, 0); // Set the stroke color for the direction line
  line(0, 0, robotSize, 0); // Draw the direction line
  strokeWeight(1);
  popMatrix(); // Restore the previous transformation matrix
}

void keyPressed() {
  if (key == 'r') {
    currentIndex = 0; // Reset the index to start over
  }
}
