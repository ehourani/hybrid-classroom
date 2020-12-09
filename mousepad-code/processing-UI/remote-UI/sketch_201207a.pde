import processing.serial.*;
import java.util.Arrays;  //can be useful for debugging
int[] rgb= new int[3];
int[] cues= new int[3];
boolean hand_raised = false;

Serial myPort;
int serialPort = 0;       //serial port that the Arduino is connected to

int count;
int flag1;

boolean raised_hand;

void setup() {
  size(1300, 800);
  noStroke();
  
  if (serialPort <= Serial.list().length) {
    myPort = new Serial(this, "/dev/cu.SLAB_USBtoUART", 115200);
    myPort.clear(); // clear the buffer
    myPort.bufferUntil(10); // always buffer until the newline symbol
  } else {
    println("Error: serial Port not found");
  }

  
  background(230);
  textSize(32);
  fill(0, 102, 200); 
  text("Good Morning, Eesam!", 430, 60);
  fill(120);
  textSize(25);
  stroke(153);
  strokeWeight(3);
  noFill();
  rect(30,150,360, 300,30);
  text("Your feedback:",120,130);
  text("Question Status:", 960, 130);
  rect(880,150,380, 300,30);
}
void draw() {
  noStroke();
  PImage img;
  img = loadImage("bear 6810.png");
  image(img, 400, 120);
  tint(rgb[0], rgb[1], rgb[2],50);
  textSize(32);
  fill(rgb[0], rgb[1], rgb[2]); 
  text("Good Morning, Eesam!", 430, 60);
  if (myPort.available() >= 0) {
    String val = myPort.readStringUntil('\n');
    println(val);
    if (val != null) {
      int rawdata[] = int(trim(val.split(","))); // unedited data
      if (rawdata.length >3) {
        rgb[0] = rawdata[0];
        rgb[1] = rawdata[1];
        rgb[2] = rawdata[2];
        cues[0] = rawdata[3];
        cues[1] = rawdata[4];
        cues[2] = rawdata[5];
      }
    if ((cues[0]==1)&&(cues[1]==0)){
      //hand raised, after x seconds lower hand
      textSize(20);
      fill(153);
      text("Raising hand, waiting for teacher.", 910, 200);
      raised_hand = true;
      fill(230);
      rect(910,340,330,100);
      count = 0;
      noFill();
      PImage hand_img;
      hand_img = loadImage("hand.png");
      hand_img.resize(120, 120);
      tint(rgb[0], rgb[1], rgb[2]);
      image(hand_img, 1000, 280);
    }
    if ((cues[0]==0)&&(raised_hand)){
      fill(230);
      rect(910,160,330,70);
      textSize(20);
      rect(1000,260,130,150);
      fill(153);
      text("Question answered.", 980, 400);
      println(millis());
      if (count==0){
        flag1 = millis();
      }
      if ((millis()>(flag1+2000))){
        fill(230);
        rect(910,360,330,70);
        println(count);
        raised_hand = false;
      }
      count++;
    }
    if (cues[1]==1){
      textSize(20);
      fill(20,200,20);
      text("Great job! High five to say thanks.", 50, 230); 
    }else{
      fill(230);
      rect(50,200,330,120);
    }
    if (cues[2]==1){
      textSize(20);
      fill(200,20,20);
      text("Please do better next time.", 80, 400); 
    }else{
      fill(230);
      rect(50,340,320,100);
    }
    //fill(rgb[0], rgb[1], rgb[2]); 
    //circle(620, 510, 200);
  }
  println(Arrays.toString(rgb));
  println(Arrays.toString(cues));
} else {
  println(myPort.available());
}
}
