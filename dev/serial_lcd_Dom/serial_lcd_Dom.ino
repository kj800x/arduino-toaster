
void setup()
{
  Serial1.begin(19200);
  Serial1.write(17);
  
  Serial1.write(12);
  delay(5);
}  

void loop()
{
  Serial1.write(12);
  delay(5);
  Serial1.write("Hello");
 Serial1.write(13); 
  Serial1.write("World"); 
  //cantina();
  delay(1000);
}

void cantina()
{
  Serial1.write(217);
  playNote(209,220);
  for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  playNote(209,225);
  for( int i = 0; i < 2; i++)
  {playNote(210,232);
  }
  playNote(209,220);
  for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  playNote(209,225);
  for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  playNote(209,220);
  playNote(209,225);
  for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  playNote(209,220);
  for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  Serial1.write(216);
   playNote(209,231);
   Serial1.write(217);
   playNote(209,220);
   for( int i = 0; i < 2; i++)
  {playNote(209,232);
  }
  playNote(211,220);
  Serial1.write(216);
   playNote(211,231);
   Serial1.write(217);
   playNote(211,220);
   Serial1.write(216);
   playNote(211,230);
}

void playNote(int time, int note)
{
  Serial1.write(time);
  
    Serial1.write(note);
}
