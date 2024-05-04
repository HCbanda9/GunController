#include <Wire.h>
#include <MPU6050.h>
#include <BleCombo.h>


//Basic IO Components Mainly Buttons
#define VRX_PIN 4
#define VRY_PIN 2
#define leftClick 15
#define relocator 33
#define space 12
#define e 13
#define r 14
#define shift 27
#define reticle 32
int xValue, yValue;

//Accelerometer Variables
MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int mouseX, mouseY;
bool reloadCounter=0;

void setup() {
  //Accelerometer Setup
  Wire.begin();
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1)
      ;  //Wait Till The Sensor Initializes
  }
  Serial.println("Sensor Intialized");
  //Setting The Offset For The Accelerometer
  mpu.setXGyroOffset(82);
  mpu.setYGyroOffset(14);
  mpu.setZGyroOffset(-35);
  mpu.setZAccelOffset(-339);
  //IO Setup
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(leftClick, INPUT_PULLUP);
  pinMode(space, INPUT_PULLUP);
  pinMode(relocator, INPUT_PULLUP);
  pinMode(shift, INPUT_PULLUP);
  pinMode(r, INPUT_PULLUP);
  pinMode(e, INPUT_PULLUP);
  //Bluetooth Setup
  Serial.begin(115200);
  Serial.println("Starting work!");
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  //Getting The Values From The Accelerometer
  Serial.println(touchRead(reticle));
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  mouseX = ((-(gx + 100) / 150));
  mouseY = (gz + 100) / 150;

  //Getting The Values From The Joystick
  xValue = analogRead(VRX_PIN);
  yValue = analogRead(VRY_PIN);


  //When Bluetooth Is Connected
  if (Keyboard.isConnected()) {
   delay(30);
    //Mouse Clicks
    if (digitalRead(relocator) == 0) {
      mouseY = 0;
      mouseX = 0;
    }
    Mouse.move(-mouseY, -mouseX);
    if (digitalRead(leftClick) == 0) {
      Mouse.press(MOUSE_LEFT);
    } else if (digitalRead(leftClick) == 1) {
      Mouse.release(MOUSE_LEFT);
    }
    
      if(digitalRead(shift)==0){
        Keyboard.press(KEY_LEFT_SHIFT);
      }
      else if(digitalRead(shift)==1){
        Keyboard.release(KEY_LEFT_SHIFT);
      }
      if(digitalRead(space)==0){
        Keyboard.press(' ');
      }
      else if(digitalRead(space)==1){
        Keyboard.release(' ');
      }

      if(digitalRead(r)==0 && reloadCounter==1){
        Keyboard.press('r');
        delay(10);
        Keyboard.release('r');
        reloadCounter=0;
      }
      else if(digitalRead(r)==1){
        Keyboard.release('r');
        reloadCounter=1;
      }
      if(digitalRead(e)==0){
        Keyboard.press('e');
      }
      else if(digitalRead(e)==1){
        Keyboard.release('e');
      }

    if (touchRead(reticle) < 50) {
      Mouse.press(MOUSE_RIGHT);
    } else if (touchRead(reticle) >= 50) {
      Mouse.release(MOUSE_RIGHT);
    }


    //Forward Input:W
    if (xValue > 4000) {
      Keyboard.press('w');
    } else if (xValue < 3800) {
      Keyboard.release('w');
    }

    //Backward Input:S
    if (xValue < 200) {
      Keyboard.press('s');
    } else if (xValue > 200) {
      Keyboard.release('s');
    }

    //Left Input:A
    if (yValue < 200) {
      Keyboard.press('a');
    } else if (yValue > 200) {
      Keyboard.release('a');
    }

    //Right Input:D
    if (yValue > 3900) {
      Keyboard.press('d');
    } else if (yValue < 3900) {
      Keyboard.release('d');
    }
  }
}
