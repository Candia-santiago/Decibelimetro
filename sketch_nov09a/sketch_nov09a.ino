#include <LiquidCrystal.h>
int MIC= A0;
int LED=9;
int interruptor = 10;
int pulsador = 13;
String ultimoEstado = "x";
String estado = "x";
const int CANTIDAD_MUESTRAS = 15;
int vector[CANTIDAD_MUESTRAS];
int promedio = 0;
int contador = 0;
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd_1(rs, en, d4, d5, d6, d7);//configurar los pines

void setup()
{
  pinMode(pulsador, INPUT);
  pinMode(interruptor, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(MIC, INPUT);
  Serial.begin(9600);
  lcd_1.begin(16, 2);
}

void loop()
{
  if (digitalRead(interruptor) == HIGH){
    readMicrofone();
   }
   else{
    Serial.println("else");
    digitalWrite(LED, LOW);
    lcd_1.clear();
   }
}


void readMicrofone(){
  int parametro1 = 0;
  int parametro2 = 0;
  int sum = 0;
  lcd_1.setCursor(0,0);
  ultimoEstado = "";
  for (int i = 0 ; i < CANTIDAD_MUESTRAS; i++)
  {
      vector[i] = analogRead(MIC);
      if ((sum + vector[i])<0)
      {
        sum = -1*(sum + vector[i]);
      }
      else
      {
        sum = sum + vector[i];
      }
  }
  promedio = sum / CANTIDAD_MUESTRAS;
  Serial.println(promedio);
  
  if (digitalRead(pulsador)==LOW)
  {
    contador= contador + 1;
    Serial.println(contador);
    if (contador >= 2){
      contador = 0;
    }
  }

  if (contador==1){
    parametro1 = 0;
    parametro2 = 0;
  } 
  else{
    if (contador == 2){
      parametro1 = 20;
      parametro2 = 20;
    }
  }
  Serial.println(parametro1);
  if (promedio>0 && promedio<=(parametro2 + 40))
  {
    estado = "estable";
    if (estado != ultimoEstado)
    {
      mostrarNivel(ultimoEstado, estado, promedio, contador);
    }
  }
  else
  {
    if (promedio>=(parametro1 + 41) && promedio<=(parametro2 + 60))
    {
      estado = "alto";
      if (estado != ultimoEstado)
      {
        mostrarNivel(ultimoEstado, estado, promedio, contador);
        digitalWrite(LED, HIGH);     
      }
    }
    else
    {
      if (promedio>=(parametro1 + 61) && promedio<=(parametro2 + 75))
      {
        estado = "muy alto";
        if (estado != ultimoEstado)
          {
          mostrarNivel(ultimoEstado, estado, promedio, contador);
          digitalWrite(LED, HIGH);
          }
      }
      else
      {
        if (promedio>(parametro2 + 75))
        {
          estado = "terrible";
          if (estado != ultimoEstado)
          {          
            mostrarNivel(ultimoEstado, estado, promedio, contador);
            digitalWrite(LED, HIGH);  
          }
        }
      }
    }
  }
  delay(100);
}

void mostrarPromedio(int promedio){
    lcd_1.setCursor(0,1);
    lcd_1.print(promedio);
}

void mostrarNivel(String ultimoEstado,String estado,int promedio,int contador){
  lcd_1.clear();
  ultimoEstado = estado;
  lcd_1.print(estado);
  mostrarPromedio(promedio);
  lcd_1.setCursor(15,1);
  lcd_1.print(contador);
}
