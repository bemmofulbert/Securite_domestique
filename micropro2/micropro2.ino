#include <Servo.h>
Servo myservo;
#include <Wire.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
#define Flame A3
#define Gas A4
#define Pir A5
#define Vib A6
#define Ir A7
#define Temp A8
#define vent 9
#define Buzzer A9
#define Switch 7
boolean Fire, Smoke, Intruder, Window, Door, Temperature;
boolean Mode = false;
#define Password_Lenght 5 // offre assez de place pour 6 charactère + le null
int pos = 0;    // variable pour enregistré la position du moteur
char Data[Password_Lenght];
char Master[Password_Lenght] = "1234";
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
Keypad customKeypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS); //initialize an instance of class NewKeypad

bool aAfficherMoteur = false;
unsigned char oldTemp = 0;
 float x;

#define PIN_MUX_C 10
#define PIN_MUX_B 11
#define PIN_MUX_A 12
//////////////////////////////////////////////////////////////////////////////////SETUP////////////////
void setup()
{
  Serial.begin(9600);
  pinMode(Flame, INPUT_PULLUP);
  pinMode(Gas, INPUT_PULLUP);
  pinMode(Pir, INPUT_PULLUP);
  pinMode(Vib, INPUT_PULLUP);
  pinMode(Ir, INPUT_PULLUP);
  pinMode(Switch, INPUT_PULLUP);
  pinMode(Temp, INPUT);
  pinMode(vent, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  lcd.init();
  lcd.backlight();
  pinMode(Buzzer, OUTPUT);
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

  setOut(PIN_MUX_C);
  setOut(PIN_MUX_B);
  setOut(PIN_MUX_A);
  desactive(PIN_MUX_A);
  desactive(PIN_MUX_B);
  desactive(PIN_MUX_C);
}
/////////////////////////////////////LOOP/////////////////////////////////////////////LOOP////////////////
void loop()
{
  gererMUX();
  if (door_state == 0)
  {
    x = analogRead(Temp);
    gererVentilateur();
    Fire = digitalRead(Flame);
    Smoke = digitalRead(Gas);
    Intruder = digitalRead(Pir);
    Window = digitalRead(Vib);
    Door = digitalRead(Ir);
    Mode = digitalRead(Switch);
    Temperature = digitalRead(Temp);
    SensorDisplay();
    SensorValues();
    if (Mode == false) // Mode normal
    {
      lcd.setCursor(4, 0);
      lcd.print("Mode Normal");
      if ((Fire == HIGH) || (Smoke == HIGH))
      {
        digitalWrite(Buzzer, HIGH);
      }
      else {
        digitalWrite(Buzzer, LOW);
      }

    } else // Mode securiser
    {
      lcd.setCursor(4, 0);

      lcd.print("Mode Securise");
      if ((Fire == HIGH) || (Smoke == HIGH) || (Intruder == HIGH) || (Window == HIGH) || (Door == HIGH))
      {
        digitalWrite(Buzzer, HIGH);
      }
      else {
        digitalWrite(Buzzer, LOW);
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

  if (data_count == Password_Lenght - 1)
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
    myservo.write(pos);              // dit au moteur d'aller à la position 'pos'
    delay(15);                       // patiente 15ms pour que le moteur arrive à la position
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
    analogWrite(vent,0);
    //Serial.println("Fan OFF");
  }
  if(x==26)
  {
    analogWrite(vent, 51);
    percent ="20%";
  }
  else if(x==27)
  {
   analogWrite(vent, 102);
   percent ="40%";
  }
  else if(x==28)
  {
   analogWrite(vent, 153);
   percent ="60%";
  }
    else if(x==29)
  {
   analogWrite(vent, 204);
   percent ="80%";
  }
      else if(x>=30)
  {
   analogWrite(vent, 255);
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
  pinMode(pin,OUTPUT);
}

//passer un pin en mode entree
void setIN(char pin){
  pinMode(pin,INPUT);
}

//active
void active(int pin){
  digitalWrite(pin,HIGH);
}
//desactive
void desactive(int pin){
  digitalWrite(pin,LOW);
}

void MUX_A_OFF(){desactive(PIN_MUX_A);}
void MUX_A_ON(){active(PIN_MUX_A);}

void MUX_B_OFF(){desactive(PIN_MUX_B);}
void MUX_B_ON(){active(PIN_MUX_B);}

void MUX_C_OFF(){desactive(PIN_MUX_C);}
void MUX_C_ON(){active(PIN_MUX_C);}

void setMUX(int etatC,int etatB,int etatA){
  if (etatA == LOW) desactive(PIN_MUX_A);
  else active(PIN_MUX_A);
  if (etatB == LOW) desactive(PIN_MUX_B);
  else active(PIN_MUX_B);
  if (etatC == LOW) desactive(PIN_MUX_C);
  else active(PIN_MUX_C);
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
