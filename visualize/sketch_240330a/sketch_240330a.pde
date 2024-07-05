Table table;
int currentIndex = 0;
float robotSize = 20; // Size of the robot for visualization

void setup() {
  size(600, 600); // Set the size of the window
  table = loadTable("../SIMULATION_TEST.csv", "header"); // Load the CSV file
}

void draw() {
  background(255); // Clear the background

  // Translate origin to the center of the window
  translate(width / 2, height / 2);
  
  // Flip the y-axis to make positive y go up
  scale(1, -1);

  // Check if we still have data to read
  if (currentIndex < table.getRowCount()) {
    TableRow row = table.getRow(currentIndex);
    
    float x = row.getFloat("x") * 100; // Adjust scale as needed
    float y = row.getFloat("y") * 100;
    float theta = radians(row.getFloat("theta"));
    
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

    // Increment the index to read the next row in the next frame
    currentIndex++;
  }
}
