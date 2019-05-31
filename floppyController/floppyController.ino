//floppy and drum counts:
#define FINST 6
#define DINST 0
//floppy stuffs:
boolean dir[FINST];
boolean step[FINST];
int steps[FINST];
long period[FINST];
long last[FINST];

//constants:
int halfMax = 75;//actual max steps
int maxSteps = halfMax * 2;//max 80 * 2
long maxDrum = 50000;//50ms: (50000) time until retract

//pin definitions: (was 24)
#define startFlop 2
//old
#define stepOff 0
#define dirOff 1
#define enOff 2
//en/step seems to be 0?
//dir seems to be 1?
//#define stepOff 2
//#define dirOff 1
//#define enOff 3
//drum suffs:
long drumTime[DINST];
int drumPin[DINST] = {};
int bellPin = A3;
int bellMod = 1000;
int bellHold = 1;
boolean drumStat[DINST] = {};
void setup()
{
  Serial.begin(115200);//max. baud
  
  int i = 0;
  //drum init 1:
  i = 0;
  while(i != DINST)
  {
    pinMode(drumPin[i], OUTPUT);
    digitalWrite(drumPin[i], false);
    i++;
  }
  //floppy init:
  i = 0;
  while(i != FINST)
  {
    dir[i] = true;
    step[i] = false;
    period[i] = 2147483647;
    last[i] = 0;
    pinMode(startFlop + stepOff + i*3, OUTPUT);//step
    pinMode(startFlop + dirOff + i*3, OUTPUT);//dir
    pinMode(startFlop + enOff + i*3, OUTPUT);//enable
    digitalWrite(startFlop + dirOff + i*3, LOW);
    digitalWrite(startFlop + enOff + i*3, LOW);
    //reset position and testing:
    int j = 0;
    while(j!= halfMax)
    {
      digitalWrite(startFlop + stepOff + i*3, HIGH);
      delay(3);
      digitalWrite(startFlop + stepOff + i*3,LOW);
      delay(3);
      j++;
    }
    digitalWrite(startFlop + enOff + i*3, HIGH);
    digitalWrite(startFlop + dirOff + i*3, dir[i]);
    steps[i] = 0;
    i++;
  }
  
  
  //drum init 2:
  i = 0;
  while(i != DINST)
  {
    digitalWrite(drumPin[i], true);
    delay(maxDrum/1000);//play each drum for testing
    digitalWrite(drumPin[i], false);
    delay(500);//make it possible to see which drive is which
    i++;
  }
}
byte inPoint = 0;
byte inData[FINST * 3 + 3];
void commit(int i)
{
  period[i] = inData[0] << 14 | inData[1] << 7 | inData[2];
  if(period[i] == 0)
  {
    period[i] = 2147483647;//basically never play a note
    digitalWrite(startFlop + enOff + i*3, HIGH);//disable
  }else digitalWrite(startFlop + enOff + i*3, LOW);//enable
  
  //experimental: flip direction for emphasis on new note
  if(steps[i] != 0 && steps[i] != maxSteps)
  {
    
  }
  inPoint = 0;
}
void cutoff(int i)
{
  period[i] = 2147483647;
  digitalWrite(startFlop + enOff + i*3, HIGH);//disable
}
void loop()
{
  //begin check serial
  while(Serial.available())
  {
    byte b = Serial.read();
    //TODO: make this modular 0-25 Fon 25-50 Foff 50-100(?) drum
    /*if(b == 0)//on 0
    {
      commit(0);
    }
    else if(b == 1)//on 1
    {
      commit(1);
    }
    else if(b == 25)//off 0
    {
      cutoff(0);
    }
    else if(b == 26)//off 1
    {
      cutoff(1);
      else if(b == 51)//drum 0
    {
      digitalWrite(drumPin[0], true);
      drumTime[0] = micros();
      drumStat[0] = true;
    }
    else if(b == 52)//drum 1
    {
      digitalWrite(drumPin[1], true);
      drumTime[1] = micros();
      drumStat[1] = true;
    }
    }*/
    if(b < 25)//floppy on
    {
      commit(b);
    }else if(b < 50)//floppy halt
    {
      cutoff(b - 26);
    }
    else if (b < 100)//drum
    {
      int pin = b - 51;
      digitalWrite(drumPin[pin], true);
      drumTime[pin] = micros();
      drumStat[pin] = true;
    }
    else//all others are data commands (TODO ensure top bit not set)
    {
      inData[inPoint] = b & 0b01111111;
      inPoint++;
    }
  }
  //end check serial
  //begin check floppies
  int i = 0;
  while(i != FINST)
  {
    if(micros() - last[i] > period[i])
    {
      last[i] = micros();
      digitalWrite(startFlop + stepOff + i*3, step[i]);
      step[i] = !step[i];
      steps[i]++;
      if(steps[i] > maxSteps)
      {
        steps[i] = 0;
        dir[i] = !dir[i];
        digitalWrite(startFlop + dirOff + i*3, dir[i]);
      }
    }
    i++;
  }
  //end check floppies
  //begin check drums
  i = 0;
  while(i != DINST)
  {
    if(drumStat[i] == true && micros() - drumTime[i] > maxDrum)
    {
      digitalWrite(drumPin[i], false);
      drumStat[i] = false;
    }
    i++;
  }
  //end check drums
}
