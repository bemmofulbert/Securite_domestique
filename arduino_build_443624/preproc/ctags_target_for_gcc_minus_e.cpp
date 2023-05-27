# 1 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino"
# 2 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino" 2
Servo myservo;
# 4 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino" 2
# 5 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino" 2
# 6 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino" 2
LiquidCrystal_I2C lcd(0x27, 20, 4);
# 16 "C:\\Users\\INITIATIC_DREAMTEK\\Documents\\micropro2\\micropro2.ino"
boolean Fire, Smoke, Intruder, Window, Door, Temperature;
boolean Mode = false;

int pos = 0; // variable pour enregistré la position du moteur
char Data[5 /* offre assez de place pour 6 charactère + le null*/];
char Master[5 /* offre assez de place pour 6 charactère + le null*/] = "1234";
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
bool door_state = true;
byte rowPins[ROWS] = {A2, A1, A0, 8}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {2, 3, 4}; //connect to the column pinouts of the keypad
Keypad customKeypad( ((char*)keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

bool aAfficherMoteur = false;
unsigned char oldTemp = 0;
 float x;




//////////////////////////////////////////////////////////////////////////////////SETUP////////////////
void setup()
{
  Serial.begin(9600);
  pinMode(A3, 0x2);
  pinMode(A4, 0x2);
  pinMode(A5, 0x2);
  pinMode(A6, 0x2);
  pinMode(A7, 0x2);
  pinMode(7, 0x2);
  pinMode(A8, 0x0);
  pinMode(9, 0x1);
  pinMode(A9, 0x1);
  lcd.init();
  lcd.backlight();
  pinMode(A9, 0x1);
  lcd.setCursor(0, 1);
  lcd.print("SYSTEME DE SECURITE");
  lcd.setCursor(0, 2);
  lcd.print(" INTELLIGENTE ");
  lcd.setCursor(6, 3);
  lcd.print("ARDUINO MEGA");
  delay(100);
  lcd.clear();
  myservo.attach(6);
  ServoClose();

  setOut(10);
  setOut(11);
  setOut(12);
  desactive(12);
  desactive(11);
  desactive(10);
}
/////////////////////////////////////LOOP/////////////////////////////////////////////LOOP////////////////
void loop()
{
  gererMUX();
  if (door_state == 0)
  {
    x = analogRead(A8);
    gererVentilateur();
    Fire = digitalRead(A3);
    Smoke = digitalRead(A4);
    Intruder = digitalRead(A5);
    Window = digitalRead(A6);
    Door = digitalRead(A7);
    Mode = digitalRead(7);
    Temperature = digitalRead(A8);
    SensorDisplay();
    SensorValues();
    if (Mode == false) // Mode normal
    {
      lcd.setCursor(4, 0);
      lcd.print("Mode Normal");
      if ((Fire == 0x1) || (Smoke == 0x1))
      {
        digitalWrite(A9, 0x1);
      }
      else {
        digitalWrite(A9, 0x0);
      }

    } else // Mode securiser
    {
      lcd.setCursor(4, 0);

      lcd.print("Mode Securise");
      if ((Fire == 0x1) || (Smoke == 0x1) || (Intruder == 0x1) || (Window == 0x1) || (Door == 0x1))
      {
        digitalWrite(A9, 0x1);
      }
      else {
        digitalWrite(A9, 0x0);
      }
    }


    customKey = customKeypad.getKey();
    if (customKey == '#')
    {
      lcd.clear();
      ServoClose();
      lcd.print("Porte fermer");
      delay(50);
      door_state = 1;
    }

  }
  else Open();

}


void Open() {
  lcd.setCursor(0, 0);
  lcd.print(" Entrer votre mot de passe");
  customKey = customKeypad.getKey();

  if (customKey) // vérifie si le chiffre à été cliqué
  {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1); // deplace le curseur
    lcd.print(Data[data_count]);
    data_count++;
  }

  if (data_count == 5 /* offre assez de place pour 6 charactère + le null*/ - 1)
  {
    if (!strcmp(Data, Master))
    {
      lcd.clear();
      ServoOpen();
      lcd.print("la porte c'est ouverte");
      lcd.clear();
      door_state = 0;
    }

    else {
      lcd.clear();

      lcd.print(" Mdp erroner");

      delay(50);

      door_state = 1;

    }
    clearData();
  }

}


void clearData()
{
  while (data_count != 0)
  {
    Data[data_count--] = 0;
  }
  return;

}

void ServoOpen()
{

  for (pos = 180; pos >= 0; pos -= 5)
  { // va de 0 à 180°
    myservo.write(pos); // dit au moteur d'aller à la position 'pos'
    delay(15); // patiente 15ms pour que le moteur arrive à la position
  }
}

void ServoClose()
{
  for (pos = 0; pos <= 180; pos += 5)
  {
    myservo.write(pos);
    delay(15);
  }
}


void SensorDisplay()
{
  lcd.setCursor(0, 1);
  lcd.print("Fire:");
  lcd.setCursor(10, 1);
  lcd.print("Smoke:");
  lcd.setCursor(0, 2);
  lcd.print("Door:");
  lcd.setCursor(10, 2);
  lcd.print("Window:");
  lcd.setCursor(0, 3);
  lcd.print("Intruder:");
}

void SensorValues()
{
  if (Fire == true)
  {
    lcd.setCursor(6, 1);
    lcd.print("Yes");
  }
  else
  {
    lcd.setCursor(6, 1);
    lcd.print("No ");
  }
  if (Smoke == true)
  {
    lcd.setCursor(17, 1);
    lcd.print("Yes");
  }
  else
  {
    lcd.setCursor(17, 1);
    lcd.print("No ");
  }
  if (Intruder == true)
  {
    lcd.setCursor(11, 3);
    lcd.print("Yes");
  }
  else
  {
    lcd.setCursor(11, 3);
    lcd.print("No ");
  }
  if (Window == true)
  {
    lcd.setCursor(17, 2);
    lcd.print("Yes");
  }
  else
  {
    lcd.setCursor(17, 2);
    lcd.print("No ");
  }
  if (Door == true)
  {
    lcd.setCursor(6, 2);
    lcd.print("Yes");
  }
  else
  {
    lcd.setCursor(6, 2);
    lcd.print("No ");
  }
}

//////////////////////////////////////////////////////////////////////////////////


void gererVentilateur(){
  String percent = "0%";
  x = (x*5)/1023;
  x = (int)(x*100);

   if(x<26)
  {
    analogWrite(9,0);
    //Serial.println("Fan OFF");
  }
  if(x==26)
  {
    analogWrite(9, 51);
    percent ="20%";
  }
  else if(x==27)
  {
   analogWrite(9, 102);
   percent ="40%";
  }
  else if(x==28)
  {
   analogWrite(9, 153);
   percent ="60%";
  }
    else if(x==29)
  {
   analogWrite(9, 204);
   percent ="80%";
  }
      else if(x>=30)
  {
   analogWrite(9, 255);
   percent ="100%";
  }

  if(!aAfficherMoteur && x!=oldTemp) {
    Serial.println(x);
    if (x<26)Serial.println("Fan OFF");
    else Serial.println("Fan Speed: "+percent);
    aAfficherMoteur=true;
    oldTemp = x;
  }else aAfficherMoteur=false;
}
/////////////////////////////////////////////////////////MUX////////////////
//passer un pin en mode sortie
void setOut(char pin){
  pinMode(pin,0x1);
}

//passer un pin en mode entree
void setIN(char pin){
  pinMode(pin,0x0);
}

//active
void active(int pin){
  digitalWrite(pin,0x1);
}
//desactive
void desactive(int pin){
  digitalWrite(pin,0x0);
}

void MUX_A_OFF(){desactive(12);}
void MUX_A_ON(){active(12);}

void MUX_B_OFF(){desactive(11);}
void MUX_B_ON(){active(11);}

void MUX_C_OFF(){desactive(10);}
void MUX_C_ON(){active(10);}

void setMUX(int etatC,int etatB,int etatA){
  if (etatA == 0x0) desactive(12);
  else active(12);
  if (etatB == 0x0) desactive(11);
  else active(11);
  if (etatC == 0x0) desactive(10);
  else active(10);
}

//allume le pin N du multiplexeur
void MUX_0(){
  setMUX(0,0,0);
}
void MUX_1(){
  setMUX(0,0,1);
}
void MUX_2(){
  setMUX(0,1,0);
}
void MUX_3(){
  setMUX(0,1,1);
}
void MUX_4(){
  setMUX(1,0,0);
}
void MUX_5(){
  setMUX(1,0,1);
}
void MUX_6(){
  setMUX(1,1,0);
}
void MUX_7(){
  setMUX(1,1,1);
}

void gererMUX(){
  //etat des charge aux bornes du multiplexeur
  //
  if(Fire) MUX_3(); else MUX_0();
  // de
  if(Smoke) MUX_4(); else MUX_0();
   if(Intruder) MUX_5(); else MUX_0();
   if(Window) MUX_6(); else MUX_0();
   if(Door) MUX_7(); else MUX_0();
  //Serial.println("------------ok2");
}
