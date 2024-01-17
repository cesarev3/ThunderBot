/*
*******************************************************
 Programa Robo Thunder
 Autor Magneto - Julho 2017
 ******************************************************
 */


// Definicao dos pinos
// Sentido dos motores de deslocamento - (Motor Esquerdo)
const int m1F = 5;
const int m1T = 6;

// Sentido do motor de direcao - (Motor Direito)
const int m2T = 10;
const int m2F = 11;

// Sonar
const int trig = 8;
const int echo = 7;

// variaveis
long duracao; // duracao pulso sensor ultrasom
long distancia; // distancia sensor ultrasom
int tempoDeslocamento = 1500; // tempo de deslocamento do robo
int tempoGiro = 1000; // tempo de giro do robo
int vM1 = 0; // Velocidade mestre do motor esquerdo (PWM)
int vM2 = 0; // Velocidade mestre do motor direito (PWM)
int v1M1 = 250; // Velocidade 1 do motor esquerdo (PWM)
int v2M1 = 200; // Velocidade 2 do motor esquerdo (PWM)
int v1M2 = 250; // Velocidade 1 do motor direito (PWM)
int v2M2 = 200; // Velocidade 2 do motor direito (PWM)

/**********************************\
 **                               **
 ** Definindo Setup dos programas **
 **                               **
 \*********************************/

void setup() { // Declaracoes

  // inicializa Serial
  Serial.begin(9600);

  // inicialisar pinos
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(m1F,OUTPUT);
  pinMode(m1T,OUTPUT);
  pinMode(m2F,OUTPUT);
  pinMode(m2T,OUTPUT);
  vM1 = v1M1;
  vM2 = v1M2;
  delay(5000); // parada para dar tempo de ligar o robo e esperar 5 segundos antes do movimento
}


/**********************************\
 **                               **
 ** Definindo Rotinas do programa **
 **                               **
 \*********************************/



// ************** Inicio Rotinas Comuns **************** 


void medirDistancia() { // Leitura do ultrasom

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracao = pulseIn(echo, HIGH); 
  distancia = duracao / 56;

}

void frenteApenas () { // deslocamento apenas para frente
  analogWrite(m1F,vM1);
  analogWrite(m1T,0);
  analogWrite(m2T,0);
  analogWrite(m2F,vM2);  
}

void atrasApenas () { // deslocamento apenas para tras
  analogWrite(m1F,0);
  analogWrite(m1T,vM1);
  analogWrite(m2T,vM2);
  analogWrite(m2F,0);  
}

void giroEsquerda () { // girar no eixo para esquerda
  analogWrite(m1F,0);
  analogWrite(m1T,vM1);
  analogWrite(m2T,0);
  analogWrite(m2F,vM2);     
}

void giroDireita () { // girar no eixo para direita
  analogWrite(m1F,vM1);
  analogWrite(m1T,0);
  analogWrite(m2T,vM2);
  analogWrite(m2F,0);  
}

void frenteEsquerda () { // deslocamento para frente e esquerda
  analogWrite(m1F,0);
  analogWrite(m1T,0);
  analogWrite(m2T,0);
  analogWrite(m2F,vM2);  
}

void frenteDireita () { // deslocamento para frente e direita
  analogWrite(m1F,vM1);
  analogWrite(m1T,0);
  analogWrite(m2T,0);
  analogWrite(m2F,0);  
}

void parado () { // sem deslocamento
  analogWrite(m1F,0);
  analogWrite(m1T,0);
  analogWrite(m2T,0);
  analogWrite(m2F,0);  
}

// ************** Final Rotinas Comuns ****************




/**********************************\
 **                               **
 **  Definindo Loop do programa   **
 **                               **
 \*********************************/

void loop() {

frenteApenas();
delay(tempoDeslocamento);
parado();
delay(2000);
giroDireita();
delay(tempoGiro);
parado();
delay(2000);
frenteApenas();
delay(tempoDeslocamento);
parado();
delay(2000);
giroEsquerda();
delay(tempoGiro);
parado();
delay(2000);
frenteApenas();
delay(tempoDeslocamento);
parado();
delay(2000);
atrasApenas();
delay(tempoDeslocamento);
parado();
delay(2000);

/*
medirDistancia();
delay(20);

 if(distancia < 25) {
    parado();
    delay(500);
    atrasApenas();
    delay(600);
    parado();
    giroEsquerda();
    delay(300);
  }
  else frenteApenas();
  
  Serial.print(" Distancia: ");
  Serial.println(distancia);
*/
  
}








