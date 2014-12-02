#include <FiniteStateMachine.h>
#include <ClickButton.h>

#define LEFT_ENABLE 3
#define LEFT_FORWARD 2
#define LEFT_BACKWARD 4

#define RIGHT_ENABLE 5
#define RIGHT_FORWARD 6
#define RIGHT_BACKWARD 7

ClickButton startButton(8, LOW, CLICKBTN_PULLUP);

#define NUMBER_OF_STATES 3
State Forward = State(go_forward, noop, noop);
State Backward = State(go_backward, noop, noop);
State Stopped = State(stop_engines, noop, noop);

FSM engineState = FSM(Stopped);

byte buttonPresses = 0;

void setup(){
  pinMode(LEFT_FORWARD, OUTPUT);
  pinMode(LEFT_BACKWARD, OUTPUT);
  pinMode(RIGHT_FORWARD, OUTPUT);
  pinMode(RIGHT_BACKWARD, OUTPUT);

  Serial.begin(9600);  //Begin serial communcation
  Serial.println("Start");
}

void loop(){
  startButton.Update();
  analogWrite(LEFT_ENABLE, 200);
  analogWrite(RIGHT_ENABLE, 200);
  if(startButton.clicks > 0){
    Serial.println(startButton.clicks);
    Serial.println("Pressed Button");
    buttonPresses = ++buttonPresses % NUMBER_OF_STATES;
    switch(buttonPresses){
      case 0: engineState.transitionTo(Forward); break;
      case 1: engineState.transitionTo(Backward); break;
      case 2: engineState.transitionTo(Stopped); break;
    }
  }
  engineState.update();
}


void go_forward(){
  debug("Forward!");
  digitalWrite(LEFT_FORWARD, HIGH);
  digitalWrite(LEFT_BACKWARD, LOW);
  digitalWrite(RIGHT_FORWARD, HIGH);
  digitalWrite(RIGHT_BACKWARD, LOW);  
}

void go_backward(){
  debug("Going Backward!");
  digitalWrite(LEFT_FORWARD,  LOW);
  digitalWrite(LEFT_BACKWARD, HIGH);

  digitalWrite(RIGHT_FORWARD,  LOW);
  digitalWrite(RIGHT_BACKWARD, HIGH);

}

void stop_engines(){
  debug("Stopped");
  digitalWrite(LEFT_FORWARD,  LOW);
  digitalWrite(LEFT_BACKWARD, LOW);

  digitalWrite(RIGHT_FORWARD,  LOW);
  digitalWrite(RIGHT_BACKWARD, LOW);

}

// some FSM states are noop
void noop(){
}

void debug(char msg[]){
  Serial.println(msg);
}
