const int LOCKED = 0;
const int WAITING = 2;
const int CHECKING = 3;
const int UNLOCKED = 4;

int state = LOCKED;
int push_handled = 0;

#define PASS_LENGTH 2
int pos = 0;
int seq[PASS_LENGTH] = {0, 0};
const int pass[PASS_LENGTH] = {2, 1};

void setup()
{
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  pinMode(2, INPUT);
  pinMode(10, INPUT);
  pinMode(9, INPUT);
  
  Serial.begin(9600);
}

void loop()
{
  
  switch(state){
    case LOCKED: {
      digitalWrite(11, HIGH);

      int close = digitalRead(2);

      pos = 0;
      
      if (close == HIGH) {
        digitalWrite(11, LOW);
        state = WAITING;
      }
      
      break;
    }
    
    case WAITING: {
      digitalWrite(12, HIGH);
      
      int btn1 = digitalRead(10);
      int btn2 = digitalRead(9);
      
      if (btn1 == LOW && btn2 == LOW) {
      	push_handled = 0;  
      }
      
      if (!push_handled) {
        int pushed = 0;
        if (btn1 == HIGH) {
      		seq[pos++] = 1;
      		push_handled = 1;
        	pushed = 1;
        }
        else if (btn2 == HIGH) {
        	seq[pos++] = 2;
      		push_handled = 1;
          	pushed = 1;
        }
        
        if (pushed) {
        	digitalWrite(12, LOW);
        	delay(200); 
        }
        
        if (pos >= PASS_LENGTH) {
        	state = CHECKING;
        }
      }
      break;
    }
    case CHECKING: {
      digitalWrite(12, LOW);
 
      int correct = 1;
      for (int i = 0; i < PASS_LENGTH; i++) {
        if (seq[i] != pass[i]) {
         	correct = 0; 
        }
      }
      
      if (correct) {
      	state = UNLOCKED;
      }
      else {
        for (int i = 0; i < 3; i++) {
        	digitalWrite(11, HIGH);
        	delay(500);
          digitalWrite(11, LOW);
        	delay(500);
        }
        state = LOCKED;
      }
      break; 
    }
    case UNLOCKED: {
      digitalWrite(13, HIGH);
      delay(5000);
      digitalWrite(13, LOW);
      state = LOCKED;
      break;
    }
   }
 }