#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

MFRC522 mfrc522(SS_PIN, RST_PIN);

SoftwareSerial mySoftwareSerial(A4, A5); // RX, TX
DFRobotDFPlayerMini myDFPlayer;

int som = 0;
String conteudo;

char* audiodescricao[] = {
  "63 AC 32 3E", // CHAVE 1
  "B0 E6 7F 25", // CHAVE 2
  "04 08 21 0A 22 68 81", // CHAVE 3
  // Adicione as chaves restantes
};

unsigned long previousMillis = 0;
const long interval = 2000; // Intervalo em milissegundos

void setup() {
  // RFID
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();
  Serial.println("Aproxime a chave.");
  Serial.println();

  // Comunicacao serial com o modulo MP3
  mySoftwareSerial.begin(9600);

  // Inicializa o modulo DFPlayer Mini
  Serial.println();
  Serial.println(F("DFRobot DFPlayer Mini"));
  Serial.println(F("Inicializando modulo DFPlayer... (3~5 segundos)"));

  if (!myDFPlayer.begin(mySoftwareSerial)) {
    Serial.println(F("Falha ao inicializar o DFPlayer Mini:"));
    Serial.println(F("1. Verifique as conexões do DFPlayer Mini"));
    Serial.println(F("2. Insira um cartão SD com arquivos de áudio"));
    while (true); // Pode ser uma boa estratégia para lidar com a falha na inicialização
  }

  Serial.println(F("Modulo DFPlayer Mini inicializado!"));
  myDFPlayer.setTimeOut(1000);
  myDFPlayer.volume(30);
  myDFPlayer.EQ(0);

  // Mostra a quantidade de arquivos no SD
  Serial.println();
  Serial.print("Numero de arquivos no cartao SD: ");
  Serial.println(myDFPlayer.readFileCounts(DFPLAYER_DEVICE_SD));
}

void loop() {
  // RFID
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  Serial.print("UID da tag :");
  conteudo = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

  conteudo.toUpperCase();

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    if (conteudo.substring(1) == audiodescricao[0]) {
        som = 1;
        Serial.println("Tocando som 1...");
        myDFPlayer.play(som);
        delay(3000);
    } else if (conteudo.substring(1) == audiodescricao[1]) {
        som = 2;
        Serial.println("Tocando som 2...");
        myDFPlayer.play(som);
        delay(3000);
    } else if (conteudo.substring(1) == audiodescricao[2]) {
        som = 3;
        Serial.println("Tocando som 3...");
        myDFPlayer.play(som);
        delay(3000);

    // Adicione condições para as chaves restantes
  }
}
}
