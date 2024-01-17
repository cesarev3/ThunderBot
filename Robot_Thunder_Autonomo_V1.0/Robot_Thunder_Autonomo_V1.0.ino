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
const int echo = 9;

// variaveis ultrasom
long duracao; // duracao pulso sensor superior
long distancia; // distancia sensor superior
int medida = 100;
int tempoGiro = 1020;

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
  digitalWrite(m1F,HIGH);
  digitalWrite(m1T,LOW);
  digitalWrite(m2T,LOW);
  digitalWrite(m2F,HIGH);  
}

void atrasApenas () { // deslocamento apenas para tras
  digitalWrite(m1F,LOW);
  digitalWrite(m1T,HIGH);
  digitalWrite(m2T,HIGH);
  digitalWrite(m2F,LOW);  
}

void giroEsquerda () { // girar no eixo para esquerda
  digitalWrite(m1F,LOW);
  digitalWrite(m1T,HIGH);
  digitalWrite(m2T,LOW);
  digitalWrite(m2F,HIGH);     
}

void giroDireita () { // girar no eixo para direita
  digitalWrite(m1F,HIGH);
  digitalWrite(m1T,LOW);
  digitalWrite(m2T,HIGH);
  digitalWrite(m2F,LOW);  
}

void frenteEsquerda () { // deslocamento para frente e esquerda
  digitalWrite(m1F,LOW);
  digitalWrite(m1T,LOW);
  digitalWrite(m2T,LOW);
  digitalWrite(m2F,HIGH);  
}

void frenteDireita () { // deslocamento para frente e direita
  digitalWrite(m1F,HIGH);
  digitalWrite(m1T,LOW);
  digitalWrite(m2T,LOW);
  digitalWrite(m2F,LOW);  
}

void parado () { // sem deslocamento
  digitalWrite(m1F,LOW);
  digitalWrite(m1T,LOW);
  digitalWrite(m2T,LOW);
  digitalWrite(m2F,LOW);  
}

// ************** Final Rotinas Comuns ****************




/**********************************\
 **                               **
 **  Definindo Loop do programa   **
 **                               **
 \*********************************/

void loop() {
delay(7000);
frenteApenas();
delay(3000);
parado();
delay(1000);
giroEsquerda();
delay(tempoGiro);
parado();
delay(1000);
frenteApenas();
delay(3000);
parado();
delay(1000);
giroEsquerda();
delay(tempoGiro);
parado();
delay(1000);
frenteApenas();
delay(3000);
parado();
delay(1000);
giroEsquerda();
delay(tempoGiro);
parado();
delay(1000);
frenteApenas();
delay(3000);
parado();
delay(1000);
giroEsquerda();
delay(tempoGiro);
parado();
delay(20000);
/*
 if(distancia < 25) {
    parado();
    delay(500);
    atrasApenas();
    delay(600);
    parado();
    giroEsquerda();
    delay(300);
  }
  
  Serial.print(" Distancia: ");
  Serial.println(distancia);
  */
  
}








