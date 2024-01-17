/*
*******************************************************
  Programa Robo Thunder
  Autor Magneto - Fevereiro 2020
  Com Módulo Bluetooth e ponte H vermelha
 ******************************************************
*/


// Definicao dos pinos
// Sentido dos motores de deslocamento - (Motor Esquerdo)
//const int m1F = 5;
//const int m1T = 6;

// Sentido do motor de direcao - (Motor Direito)
//const int m2T = 10;
//const int m2F = 9;

const int mA1 = 7; // Sentido dos motores de deslocamento - (Motor A)
const int mA2 = 8; // Sentido dos motores de deslocamento - (Motor A)

const int mB1 = 11; // Sentido dos motores de deslocamento - (Motor B)
const int mB2 = 6; // Sentido dos motores de deslocamento - (Motor B)

const int vMA = 9; // Velocidade do motor A (PWM)
const int vMB = 10; // Velocidade do motor B (PWM)

// Sonar
const int trig = 5;
const int echo = 4;

// Leds
const int ledAmarelo = 12;
const int ledVerde = 13;
const int farol = 3;

// entradas
const int estado = A1;
const int ldr = A0;

// variaveis
long duracao; // duracao pulso sensor ultrasom
long distancia; // distancia sensor ultrasom
int tempoDeslocamento = 1500; // tempo de deslocamento do robo
int tempoGiro = 1000; // tempo de giro do robo
int velocidadeMA = 200; // velocidade do motor A (<= 255)
int velocidadeMB = 200; // velocidade do motor B (<= 255)
int fixaFarol = 0; // se estiver em 1 deixa o farol aceso

byte autonomo = 0; // identificar se esta no modo autonomo ou celular zero = autonomo e 1 = celular
int luz = 1000; // armazena a luminosidade vinda do LDR
int lerEstado = 0; // armazenar valor lido no pino de estado

// variavel de recepcao de dados bluetooth
int incomingByte; // Variavel de entrada de dados pela Serial

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
  pinMode(mB1, OUTPUT); // inicialisando pinos
  pinMode(mB2, OUTPUT); // inicialisando pinos
  pinMode(mA1, OUTPUT); // inicialisando pinos
  pinMode(mA2, OUTPUT); // inicialisando pinos
  pinMode(vMA, OUTPUT); // inicialisando pinos
  pinMode(vMB, OUTPUT); // inicialisando pinos

  //pinMode(m1F, OUTPUT);
  //pinMode(m1T, OUTPUT);
  //pinMode(m2F, OUTPUT);
  //pinMode(m2T, OUTPUT);

  pinMode(ledAmarelo, OUTPUT);
  pinMode(ledVerde, OUTPUT);
  pinMode(farol, OUTPUT);
  pinMode(estado, INPUT);

  analogWrite(vMA, velocidadeMA); // registrando a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registrando a velocidade do motor B

  // vM1 = v1M1;
  // vM2 = v1M2;
  //delay(5000); // parada para dar tempo de ligar o robo e esperar 5 segundos antes do movimento
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
  //delay(100);

}

void frenteApenas () { // deslocamento apenas para frente
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, HIGH);
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, HIGH);
}

void atrasApenas () { // deslocamento apenas para tras
  digitalWrite(mB1, HIGH);
  digitalWrite(mB2, LOW);
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, LOW);
}

void giroEsquerda () { // girar no eixo para esquerda
  digitalWrite(mB1, HIGH);
  digitalWrite(mB2, LOW);
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, HIGH);
}

void giroDireita () { // girar no eixo para direita
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, HIGH);
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, LOW);
}

void frenteEsquerda () { // deslocamento para frente e esquerda
  digitalWrite(mA1, LOW);
  digitalWrite(mA2, HIGH);
}

void frenteDireita () { // deslocamento para frente e direita
  digitalWrite(mB1, LOW);
  digitalWrite(mB2, HIGH);
}

void parado () { // sem deslocamento
  digitalWrite(mB1, HIGH);
  digitalWrite(mB2, HIGH);
  digitalWrite(mA1, HIGH);
  digitalWrite(mA2, HIGH);
}

void acenderFarol () { // acender Farol
  digitalWrite(farol, HIGH);
}

void apagarFarol () { // apagar Farol
  digitalWrite(farol, LOW);
}

// ************** Final Rotinas Comuns ****************


// ************** Inicio Rotina Modo Autonomo ****************

void modoAutonomo () { // modo Autonomo

  velocidadeMA = 70; // velocidade do motor A (<= 255)
  velocidadeMB = 70; // velocidade do motor B (<= 255)
  analogWrite(vMA, velocidadeMA); // registrando a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registrando a velocidade do motor B

  medirDistancia();
  delay(50);

  if (distancia < 50) {
    parado();
    delay(500);
    atrasApenas();
    delay(600);
    parado();
    giroEsquerda();
    delay(250);
  }
  else frenteApenas();
  delay(150);
}

// ************** Final Rotina Modo Autonomo ****************


// ************** Inicio Rotina Modo Celular ****************

void modoCelular () { // Modo Celular

  velocidadeMA = 200; // velocidade do motor A (<= 255)
  velocidadeMB = 200; // velocidade do motor B (<= 255)
  analogWrite(vMA, velocidadeMA); // registrando a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registrando a velocidade do motor B

  if (fixaFarol == 1) {
    acenderFarol();
  }
  else {
    apagarFarol();
  }

  if (Serial.available() > 0) { // Verificando se tem entrada de dados na Serial e se está no modo celular
    incomingByte = Serial.read(); // Lendo o ultimo dado recebido

    if (incomingByte == 'U') {
      frenteApenas();
      delay(50);
    }
    else if (incomingByte == 'D') {
      atrasApenas();
      delay(50);
    }
    else if (incomingByte == 'L') {
      frenteDireita();
      delay(50);
    }
    else if (incomingByte == 'R') {
      frenteEsquerda();
      delay(50);
    }
    else if (incomingByte == 'X') {
      giroDireita();
      delay(50);
    }
    else if (incomingByte == 'B') {
      giroEsquerda();
      delay(50);
    }
    else if (incomingByte == 'Y') {
      fixaFarol = 1;
      delay(50);
    }
    else if (incomingByte == 'A') {
      fixaFarol = 0;
      delay(50);
    }
  }
}

// ************** Final Rotina Modo Celular ****************

/**********************************\
 **                               **
 **  Definindo Loop do programa   **
 **                               **
  \*********************************/

void loop() {

  lerEstado = analogRead(estado); // lê a informação da chave de seleção Autonomo e Celular
  luz = analogRead(ldr); // lê a informação do sensor de luz

  if (luz < 650) { // testa se esta escuro ou claro para acender farois
    acenderFarol();
  }
  else {
    apagarFarol();
  }

  if (lerEstado < 900) { // testa se o carro esta no modo autonomo ou celular
    autonomo = 1;
  }
  else {
    autonomo = 0;
  }

  if (autonomo == 0) { // Verificando se está no modo celular
    digitalWrite(ledAmarelo, HIGH);
    digitalWrite(ledVerde, LOW);
    parado();
    modoCelular();
  }
  else if (autonomo == 1) { // Verificando se está no modo autonomo
    digitalWrite(ledAmarelo, LOW);
    digitalWrite(ledVerde, HIGH);
    modoAutonomo();
  }


  /*
    Serial.print(" Distancia: ");
    Serial.println(distancia);
    Serial.print("luz: ");
    Serial.println(luz);
    Serial.print("autonomo: ");
    Serial.println(autonomo);
     Serial.print("Ler estado: ");
    Serial.println(lerEstado);
    delay(500);
  */
}
