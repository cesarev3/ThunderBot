/*************************************************************
  Programa Thunder Tank DS (Kit da Easy DS robótica educacional) 
  Modo autônomo e controle remoto pelo celular (bluetooth) 
  utilizado Arduíno UNO, modulo Bluetooth (low energy - IOS)
  ponte H L298N (vermelha) e bateria lipo 3S 730mA a 850mA
  alarme de baixa tensão para proteger a bateria (9V)
  Autor: Carlos Cesar Pereira (Magneto) - Fevereiro 2020
  maiores detalhes no site hobbynaveia.com.br
 ************************************************************/


// Definicao dos pinos

const int mA1 = 7; // Sentido dos motores de deslocamento - (Motor A)
const int mA2 = 8; // Sentido dos motores de deslocamento - (Motor A)

const int mB1 = 11; // Sentido dos motores de deslocamento - (Motor B)
const int mB2 = 6; // Sentido dos motores de deslocamento - (Motor B)

const int vMA = 9; // Velocidade do motor A (PWM)
const int vMB = 10; // Velocidade do motor B (PWM)

// Sonar
const int trig = 5; // emissão do ultrasom
const int echo = 4; // recepção do ultrasom

// Leds
const int ledAmarelo = 12; // informa modo celular ligado
const int ledVerde = 13; // informa modo autonomo ligado
const int farol = 3;

// Buzzer
const int buzzer = 2; // apita quando a tensão da bateria baixa de 9V

// Sensores
const int estado = A1; // chave seletora do modo autonomo e celular
const int ldr = A0; // sensor de iluminação para acendimento automático dos faróis
const int tensao = A2; // recebe a informação da tensão da bateria

// Variaveis
long duracao; // recebe duracao do pulso do sensor ultrasom
long distancia; // recebe distancia do sensor ultrasom

int velocidadeMA = 70; // recebe velocidade do motor A (<= 255)
int velocidadeMB = 70; // recebe velocidade do motor B (<= 255)

float valorTensao = 12; // recebe valor da tensao convertido para Volts
int valorLdr = 1000; // recebe a valor lido do pino de entrada do LDR
int valorEstado = 0; // recebe o valor lido no pino de entrada da chave de seleção autônomo e celular

byte autonomo = 0; // identificar modo a ser selecionado: 1 = autonomo e 0 = celular
byte aceso = 0; // identifica se os faróis foram ligados ou desligados em algum momento
int fixaFarol = 0; // contador para controlar o acendimento e desligamento em um só botão do celular

int incomingByte; // recebe o byte da entrada de dados pela serial (Bluetooth)


/****************************************************
 ***      Definindo as Rotinas do programa que    ***
 ***      só serão executadas uma única vez       ***
 ***************************************************/

void setup() { // Declaracoes que são executadas apenas uma vez ao ligar o Arduino

  // inicializa Serial
  Serial.begin(9600); // abre a comunicação serial para dados para celular e debug

  // inicialisar pinos
  pinMode(trig, OUTPUT); // definindo como saída
  pinMode(echo, INPUT); // definindo como entrada
  pinMode(mB1, OUTPUT); // definindo como saída
  pinMode(mB2, OUTPUT); // definindo como saída
  pinMode(mA1, OUTPUT); // definindo como saída
  pinMode(mA2, OUTPUT); // definindo como saída
  pinMode(vMA, OUTPUT); // definindo como saída
  pinMode(vMB, OUTPUT); // definindo como saída


  pinMode(ledAmarelo, OUTPUT); // definindo como saída
  pinMode(ledVerde, OUTPUT); // definindo como saída
  pinMode(farol, OUTPUT); // definindo como saída
  pinMode(estado, INPUT); // definindo como entrada
  pinMode(buzzer, OUTPUT); // definindo como saída
  pinMode(tensao, INPUT); // definindo como entrada

  analogWrite(vMA, velocidadeMA); // registrando a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registrando a velocidade do motor B

}


/****************************************************
 ***      Definindo as Rotinas do programa        ***
 ***     só serão executadas quando chamadas      ***
 ***  no void loop ou dentro das mesmas rotinas   ***
 ***************************************************/



/****************************************************
 *******        Inicio Rotinas Comuns        ********
 ***************************************************/

void medirDistancia() { // Leitura do ultrasom

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duracao = pulseIn(echo, HIGH); // recebe a informação de leitura do sensor
  distancia = duracao / 56; // converte para centimetros
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

void alarmeTensao () { // rotina de alarme se tensão da bateria estiver baixa
  parado();
  digitalWrite(buzzer, HIGH); // <===== Recomendo comentar esta linha ENQUANTO estiver ajustando o código
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledAmarelo, HIGH);
  acenderFarol(); // chama rotina de acender farol
  delay(1000);
  digitalWrite(buzzer, LOW);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledAmarelo, LOW);
  apagarFarol(); // chama rotina de apagar farol
  delay(1000);
}

/****************************************************
 *******        Final  Rotinas Comuns        ********
 ***************************************************/


/****************************************************
 *******    Início Rotina modo autônomo      ********
 ***************************************************/

void modoAutonomo () { // modo Autonomo

  velocidadeMA = 70; // define velocidade do motor A (<= 255)
  velocidadeMB = 70; // define velocidade do motor B (<= 255)
  analogWrite(vMA, velocidadeMA); // registra a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registra a velocidade do motor B

  medirDistancia(); // executa rotina de medição da distância e dá um tempo de 50 milisegundos
  delay(50);

  if (distancia < 50) { //executa o algoritmo de desvio se encontrar obstáculo a menos de 50cm
    parado(); // para
    delay(500); // fica 500 milisegundos parado
    atrasApenas(); // vai para trás por 600 milisegundos
    delay(600);
    parado(); // para
    giroEsquerda(); // gira para a esquerda por 250 milisegundos
    delay(250);
  }
  else frenteApenas(); // segue em frente se não tiver obstáculos por 150 milisegundos
  delay(150);
}

/****************************************************
 *******     Final Rotina modo autônomo      ********
 ***************************************************/


/****************************************************
 *******     Início Rotina modo celular      ********
 ***************************************************/

void modoCelular () { // Modo Celular

  if (velocidadeMA > 220) { // especifica o limite de velocidade em 220 para motor A 
    velocidadeMA = 70; // se tentar ser maior que 220 volta a 70
  }

  if (velocidadeMB > 220) { // especifica o limite de velocidade em 220 para motor B
    velocidadeMB = 70; // se tentar ser maior que 220 volta a 70
  }

  analogWrite(vMA, velocidadeMA); // registrando a velocidade do motor A
  analogWrite(vMB, velocidadeMB); // registrando a velocidade do motor B

  if (Serial.available() > 0) { // Verificando se tem entrada de dados na Serial e se está no modo celular
    incomingByte = Serial.read(); // Lendo o ultimo dado recebido

    if (incomingByte == 'U') { // se receber U segue em frente
      frenteApenas();
      delay(50);
    }
    else if (incomingByte == 'D') { // se receber D segue para trás
      atrasApenas();
      delay(50);
    }
    else if (incomingByte == 'L') { // se receber L segue para a esquerda
      frenteDireita();
      delay(50);
    }
    else if (incomingByte == 'R') { // se receber R segue para direita
      frenteEsquerda();
      delay(50);
    }
    else if (incomingByte == 'X') { // se receber X gira no mesmo lugar para direita
      giroDireita();
      delay(50);
    }
    else if (incomingByte == 'B') { // se receber B gira no mesmo lugar para esquerda
      giroEsquerda();
      delay(50);
    }
    else if (incomingByte == 'Y') { // se receber Y adiciona 1 ao contador de farol
      fixaFarol = fixaFarol + 1;
      delay(50);
    }
    else if (incomingByte == 'A') { // se receber A muda a velocidade adicionando 30 unidades aos motores
      velocidadeMA = velocidadeMA + 30; // aumenta a velocidade do motor A em 30 unidades
      velocidadeMB = velocidadeMB + 30; // aumenta a velocidade do motor B em 30 unidades
      delay(70);
    }
  }
}

/****************************************************
 *******      Final Rotina modo celular      ********
 ***************************************************/


/****************************************************
 ***       Início do loop que será executado      ***
 ***          o tempo todo pelo Arduino           ***
 ***************************************************/

void loop() { // estas instruções se repetirão enquanto houver energia alimentando o Arduino

  valorEstado = analogRead(estado); // lê a informação da chave de seleção Autonomo e Celular
  valorLdr = analogRead(ldr); // lê a informação do sensor de luz
  valorTensao = analogRead(tensao); // lê a tensão de entrada na porta da do arduino
  valorTensao = (valorTensao * 16.65) / 1024; // converte a tensao lida na tensão real de acordo com as especificações do Arduino e divisor de tensão

  if (valorTensao <= 9) { // faz bloqueio se a tensão da bateria for igual ou menor que 9V (3V por célula)
    alarmeTensao(); // chama rotina de alarme
  }

  if (valorLdr < 650) { // testa se esta escuro ou claro para acender farois
    acenderFarol(); // chama a rotina de acender faróis se baixa luminosidade
    aceso = 1; // marca em 1 para saber que os faróis foram ligados em algum momento
  }
  else { // em caso de estar claro apaga os faróis
    apagarFarol(); // chama rotina de apagar os faróis
    aceso = 0; // marca em 0 para saber que os faroes foram desligados em algum momento
  }

  if (fixaFarol > 0 && aceso == 0) { // para evitar conflito os faróis não são acionados pelo celular se já estirerem acesos
    acenderFarol(); // depois de testar se estava apagado e se o botão de acionamento foi acionado chama rotina de acender faróis
    aceso = 1; //marca em 1 para saber que os faróis foram ligados em algum momento
  }

  if (valorEstado < 900) { // testa se chave superior esta no modo autonomo ou celular
    autonomo = 1; // marca em 1 o modo autonomo
  }
  else {
    autonomo = 0; // em caso negativo marca em 0 o modo autonomo
  }

  if (fixaFarol > 3) { // testa contador do acendimento de farol. Se for maior que 2 volta a 0. Assim acendemos e apagamos o faróis em um só botão
    fixaFarol = 0; // marca em 0 o contador do farol
  }

  if (autonomo == 0) { // Verifica se está no modo celular
    digitalWrite(ledAmarelo, HIGH); // em caso positivo liga o led amarelo e apaga o verde
    digitalWrite(ledVerde, LOW);
    parado(); // para o veículo para maior segurança 
    modoCelular(); // chama a rotina modo celular
  }
  else if (autonomo == 1) { // Verificando se está no modo autonomo
    digitalWrite(ledAmarelo, LOW); // em caso positivo liga o led verde e apaga o amarelo
    digitalWrite(ledVerde, HIGH);
    modoAutonomo(); // chama a rotina modo autônomo
  }


  Serial.println(velocidadeMA); // manda a informação da velocidade do motor A para a tela do celular
  // Serial.println(velocidadeMB); // não é necessário mandar do motor B porque são as mesmas


 /****************************************************
 ***       Início da área de debug que tem a       ***
 *** função de mostrar o conteúdo das variáveis    ***
 ***   deve ficar desligada após os testes para    ***
 ***  evitar atraso no processamento do programa   ***  
 ***************************************************/

  /*
    Serial.print(" Distancia: ");
    Serial.println(distancia);

    Serial.print("luz: ");
    Serial.println(valorLdr);

    Serial.print("autonomo: ");
    Serial.println(autonomo);
    Serial.print("Ler estado: ");
    Serial.println(valorEstado);

    Serial.print("Ler tensao: ");
    Serial.println(tensao);

    Serial.print("Fixa Farol: ");
    Serial.println(fixaFarol);

    Serial.print("Velocidade M-A: ");
    Serial.println(velocidadeMA);

    Serial.print("Velocidade M-B: ");
    Serial.println(velocidadeMB);

    delay(100); // Atenção: esta linha deverá ser utilizado só no DEBUG ou afetará o tempo de sinal no modo Celular e dará erro
  */
} // parâmetro final da instrução void loop e não pode ser apagada ou comentada

/****************************************************
 *******         Final  do código :)         ********
 ***************************************************/
