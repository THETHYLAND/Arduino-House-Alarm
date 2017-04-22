import processing.serial.*;
Serial myPort;
int val;
void setup() 
{
  size(900, 700);
  smooth();
  String portName = Serial.list()[0];
  myPort = new Serial(this, portName, 9600);
}
void draw()
{
  background(150);
  fill(255);
  rect(200, 200, 600, 400);
  rect(200, 400, 300, 200);
  rect(500, 400, 300, 200);
  rect(800, 450, 20, 150);
  triangle(200, 200, 500, 80, 800, 200);
  rect(450, 150, 100, 50);
  fill(255);
  ellipse(775, 510, 25, 25);
  fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Hallway",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  if ( myPort.available() > 0) {  
    val = myPort.read();
  }         
  if (val == 1) {       //Bedroom       
    fill(255);   
    rect(200, 400, 300, 200);
    rect(500, 400, 300, 200);
    rect(800, 450, 20, 150);
    triangle(200, 200, 500, 80, 800, 200);
    rect(450, 150, 100, 50);
    ellipse(775, 510, 25, 25);
    fill(0, 255, 0);
    rect(200, 200, 600, 200);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
  if (val==2) {        //Panic Button
    fill(255);
    rect(200, 200, 600, 400);  
    rect(200, 400, 300, 200);
    rect(500, 400, 300, 200);
    rect(800, 450, 20, 150);
    triangle(200, 200, 500, 80, 800, 200);
    rect(450, 150, 100, 50);
    fill(0, 0, 255);
    ellipse(775, 510, 25, 25);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
  if (val==4) {        //Kitchen
    fill(255);
    rect(200, 200, 600, 400);     
    rect(500, 400, 300, 200);
    rect(800, 450, 20, 150);
    triangle(200, 200, 500, 80, 800, 200);
    rect(450, 150, 100, 50);
    ellipse(775, 510, 25, 25);
    fill(0, 255, 0);
    rect(200, 400, 300, 200);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
  if (val==3) {        //Hallway
    fill(255);
    rect(200, 200, 600, 400);    
    rect(200, 400, 300, 200);
    rect(800, 450, 20, 150);
    triangle(200, 200, 500, 80, 800, 200);
    rect(450, 150, 100, 50);
    ellipse(775, 510, 25, 25);
    fill(0, 255, 0);
    rect(500, 400, 300, 200);
    fill(255);
    ellipse(775, 510, 25, 25);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
  if (val==5) {        //Front Door
    fill(255);
    rect(200, 200, 600, 400);  
    rect(200, 400, 300, 200);
    rect(500, 400, 300, 200);
    triangle(200, 200, 500, 80, 800, 200);
    rect(450, 150, 100, 50);
    ellipse(775, 510, 25, 25);
    fill(0, 255, 0);
    rect(800, 450, 20, 150);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
  if (val==6) {        //Water Tank
    fill(255);
    rect(200, 200, 600, 400);  
    rect(200, 400, 300, 200);
    rect(500, 400, 300, 200);
    triangle(200, 200, 500, 80, 800, 200);
    rect(800, 450, 20, 150);
    ellipse(775, 510, 25, 25);
    fill(255, 0, 0);
    rect(450, 150, 100, 50);
    fill(0);
  text("Water Tank", 470, 175);
  text("Bedroom",470,300);
  text("Kitchen",325,500);
  text("Kitchen",625,500);
  text("Panic Switch",730,535);
  text("Entry/Exit door",820,535);
  }
}