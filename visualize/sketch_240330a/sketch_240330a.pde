Table table;
int currentIndex = 0;
float robotSize = 20; // Size of the robot for visualization
ArrayList<PVector> waypoints = new ArrayList<PVector>();

void setup() {
  size(800, 800); // Set the size of the window
  table = loadTable("../../logs/SIMULATION_TEST.csv", "header"); // Load the CSV file
  
  // Read the table and store waypoints
  for (TableRow row : table.rows()) {
    int logType = row.getInt(0); // Assuming the first column is logType
    if (logType == 1) { // Waypoint log type
      float x = row.getFloat(2) ; // Third column is x
      float y = row.getFloat(3) ; // Fourth column is y
      waypoints.add(new PVector(x, y));
    }
  }
  println(waypoints);
}

void draw() {
  background(255); // Clear the background

  // Translate origin to the center of the window
  translate(width / 2, height / 2);
  
  // Flip the y-axis to make positive y go up
  scale(1, -1);

  // Draw waypoints
  for (PVector waypoint : waypoints) {
    fill(0, 255, 255); // Set fill color for waypoints
    ellipse(waypoint.x, waypoint.y, 10, 10); // Draw waypoint as a circle
  }

  // Check if we still have data to read
  if (currentIndex < table.getRowCount()) {
    TableRow row = table.getRow(currentIndex);
    
    int logType = row.getInt(0); // Assuming the first column is logType
    if (logType == 0) { // Vehicle state log type
      float x = row.getFloat(2) * 100; // Third column is x
      float y = row.getFloat(3) * 100; // Fourth column is y
      float theta = radians(row.getFloat(4)); // Fifth column is theta
      
      // Draw the robot
      pushMatrix(); // Save the current transformation matrix
      translate(x, y); // Move to the robot's position
      rotate(theta); // Rotate to the robot's orientation
      fill(100, 100, 250); // Set the fill color for the robot
      rectMode(CENTER); // Draw the rectangle from its center
      rect(0, 0, robotSize, robotSize); // Draw the robot as a rectangle
      stroke(255, 0, 0); // Set the stroke color for the direction line
      line(0, 0, robotSize / 2, 0); // Draw the direction line
      popMatrix(); // Restore the previous transformation matrix
    }

    // Increment the index to read the next row in the next frame
    currentIndex++;
    delay(500);
  }
}
