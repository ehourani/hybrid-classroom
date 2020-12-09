import http.requests.*;

int flag;
int count=0;

void setup() {
  size(1300, 600);
  background(230);
  noStroke();
  PImage img;
  img = loadImage("animal menu.png");
  img.resize(400, 0);
  image(img, 450, 120);
}

void draw() {
  fill(30,30,230);
  textSize(32);
  text("Choose a character for the classroom:", 370, 80);
  if ((mousePressed)&&(mouseX>550)&&(mouseX<750)&&(mouseY>100)&&(mouseY<300)){
    GetRequest get = new GetRequest("https://hybrid-classroom.herokuapp.com/animal?choice=bear");
    get.send();
    println("Reponse Content: " + get.getContent());
    println("Reponse Content-Length Header: " + get.getHeader("Content-Length"));
    flag=millis();
    fill(120,120,230);
    textSize(25);
    text("Your choice has been saved! Your teacher should receive the files for fabrication.", 200, 450);
    count++;
  }
  if (millis()>flag+3000){
    fill(230);
    rect(190,430,1000,100);
    count=0;
  }
}
