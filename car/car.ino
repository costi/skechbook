#include <FiniteStateMachine.h>
#include <ClickButton.h>
#include <IRremote.h>

#define LEFT_ENABLE    5    // leave pin 11 for the IR library
#define LEFT_BACKWARD  10
#define LEFT_FORWARD   9

#define RIGHT_BACKWARD 8
#define RIGHT_FORWARD  7
#define RIGHT_ENABLE   6  // has to be PWM

#define START_BUTTON_PIN 12

#define IR_PIN 4 // do not put PWM on pins 11 or 3 because the IR uses them

#define IR_FORWARD   0x511DBB
#define IR_BACKWARD  0xA3C8EDDB
#define IR_RIGHT     0x52A3D41F
#define IR_LEFT      0x52A3D41F
#define IR_OK        0xD7E84B1B

#define MAX_SPEED 255
#define MIN_SPEED -255

ClickButton startButton(START_BUTTON_PIN, LOW, CLICKBTN_PULLUP);

State Forward = State(go_forward, noop, noop);
State Backward = State(go_backward, noop, noop);
State Stopped = State(stop_engines, noop, noop);

FSM engineState = FSM(Stopped);
int engineSpeed = 200; // TODO put it as a variable in engineState

IRrecv ir(IR_PIN);
decode_results ir_results;
unsigned long lastIrButton = -1;

//stolen from http://www.utopiamechanicus.com/article/low-memory-serial-print/
void StreamPrint_progmem(Print &out,PGM_P format,...)
{
  // program memory version of printf - copy of format string and result share a buffer
  // so as to avoid too much memory use
  char formatString[128], *ptr;
  strncpy_P( formatString, format, sizeof(formatString) ); // copy in from program mem
  // null terminate - leave last char since we might need it in worst case for result's \0
  formatString[ sizeof(formatString)-2 ]='\0';
  ptr=&formatString[ strlen(formatString)+1 ]; // our result buffer...
  va_list args;
  va_start (args,format);
  vsnprintf(ptr, sizeof(formatString)-1-strlen(formatString), formatString, args );
  va_end (args);
  formatString[ sizeof(formatString)-1 ]='\0';
  out.print(ptr);
}
#define Serialprint(format, ...) StreamPrint_progmem(Serial,PSTR(format),##__VA_ARGS__)
#define Streamprint(stream,format, ...) StreamPrint_progmem(stream,PSTR(format),##__VA_ARGS__)

void setup(){
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);

  ir.enableIRIn(); // start the receiver

  Serial.begin(9600);  //Begin serial communcation
  Serial.println("Start");
}

void loop(){
  unsigned long irValue;
  startButton.Update();

  if(ir.decode(&ir_results)){
    irValue = ir_results.value;
    ir.resume();
    handleIRAction(irValue);
  }


  handleStartButtonClick();

  engineState.update();
}

void handleIRAction(unsigned long value){
  Serialprint("Handling IR Action %lX value\n\r", value);
  if(value == REPEAT){
    value = lastIrButton;
  } else {
    lastIrButton = value;
  }
  switch(value){
    case IR_FORWARD:
      go(calculateNewSpeed(+10));
      break;
    case IR_BACKWARD:
      go(calculateNewSpeed(-10));
      break;
    case IR_OK:
      toggleEngine();
      break;
  }
}

void handleStartButtonClick(){
  if(startButton.clicks != 0){
    Serialprint("Pressed Button %d\n", startButton.clicks);
    toggleEngine();
  }
}

void toggleEngine(){
  if(engineState.isInState(Stopped)){
    go(50);
  } else {
    go(0);
  }
}

int calculateNewSpeed(int change){
  int tillMax = MAX_SPEED - engineSpeed;
  int tillMin = engineSpeed - MIN_SPEED;
  if(change < 0 && abs(change) > tillMin){
    change = -tillMin;
  } else if(change > 0 && abs(change) > tillMax) {
    change = tillMax;
  }
  return engineSpeed + change;
}

void set_speed(int speed){
  engineSpeed = speed;
  Serialprint("Setting speed of %d\n\r", engineSpeed);
  analogWrite(LEFT_ENABLE,  abs(engineSpeed));
  analogWrite(RIGHT_ENABLE, abs(engineSpeed));
}

void go(int speed){
  set_speed(speed);
  if(speed > 0){
    engineState.transitionTo(Forward);
  } else if(speed < 0){
   engineState.transitionTo(Backward);
  } else {
    engineState.transitionTo(Stopped);
  }
}

void go_forward(){
  Serial.println("Forward!");
  //return;

  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(LEFT_BACKWARD, LOW);

  digitalWrite(RIGHT_FORWARD, HIGH);
  digitalWrite(RIGHT_BACKWARD, LOW);  
}

void go_backward(){
  Serial.println("Backward!");
  //return;

  digitalWrite(LEFT_FORWARD,  LOW);
  digitalWrite(LEFT_BACKWARD, HIGH);

  digitalWrite(RIGHT_FORWARD,  LOW);
  digitalWrite(RIGHT_BACKWARD, HIGH);

}

void stop_engines(){
  Serial.println("Stopped");

  digitalWrite(LEFT_FORWARD,  LOW);
  digitalWrite(LEFT_BACKWARD, LOW);

  digitalWrite(RIGHT_FORWARD,  LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);

}

// some FSM states are noop
void noop(){
}

