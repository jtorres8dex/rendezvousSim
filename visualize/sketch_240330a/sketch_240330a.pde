Table table;
int currentIndex = 0;
float robotSize = 20; // Size of the robot for visualization
ArrayList<PVector> waypoints = new ArrayList<PVector>();

void setup() {
  size(800, 800); // Set the size of the window
  table = loadTable("../../logs/SIMULATION_TEST.csv", "header"); // Load the CSV file

  // Print the number of rows in the table for debugging
  println("Total rows in table: " + table.getRowCount());

  // Read the table and store waypoints
  for (TableRow row : table.rows()) {
    int logType = row.getInt(0); // Assuming the first column is logType
    if (logType == 1) { // Waypoint log type
      float x = row.getFloat(2); // Scaling for visualization
      float y = row.getFloat(3);
      waypoints.add(new PVector(x, y)); // Store waypoint
      println("Added waypoint at (" + x + ", " + y + ")"); // Debugging
    }
  }
}

void draw() {
  background(255); // Clear the background each frame

  // Translate origin to the center of the window
  translate(width / 2, height / 2);
  
  // Flip the y-axis to make positive y go up
  scale(1, -1);

  // Draw waypoints
  fill(0, 255, 255); // Set fill color for waypoints
  for (PVector waypoint : waypoints) {
    ellipse(waypoint.x, waypoint.y, 10, 10); // Draw waypoint as a circle
    println("Drawing waypoint at (" + waypoint.x + ", " + waypoint.y + ")"); // Debugging
  }

  // Check if we still have data to read
  if (currentIndex < table.getRowCount()) {
    TableRow row = table.getRow(currentIndex);

    int logType = row.getInt(0); // Assuming the first column is logType
    if (logType == 0) { // Vehicle state log type
      float x = row.getFloat(2); // Third column is x
      float y = row.getFloat(3); // Fourth column is y
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
      println("Drawing robot at (" + x + ", " + y + ") with theta: " + degrees(theta)); // Debugging
    }

    // Increment the index to read the next row in the next frame
    currentIndex++;
  } else {
    println("All rows processed."); // Debugging
  }
}

void keyPressed() {
  if (key == 'r') {
    currentIndex = 0; // Reset the index to start over
  }
}
