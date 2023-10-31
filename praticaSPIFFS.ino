#include <FS.h>
#include "SPIFFS.h"
#include <NTPClient.h> /* https://github.com/arduino-libraries/NTPClient */
#include <WiFi.h> 


#define wifi_ssid "imd0902"
#define wifi_password "imd0902iot"

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000);/* Cria um objeto "NTP" com as configurações.utilizada no Brasil */
String hora;

int nowTime, oldTime = 0;

int buttonPin = 21;
int ledPin = 22;


String ledState;
String str;
String s;

int num = 0;



void writeFile(String state, String path, String hora);
String readFile(String path);
void formatFile();
void openFS(void);



void setup() {

  Serial.begin(115200);
  Serial.println("inicio");

  WiFi.begin(wifi_ssid, wifi_password);
  delay(2000);               /* Espera a conexão. */
  ntp.begin();               /* Inicia o protocolo */
  ntp.forceUpdate();    /* Atualização */

  delay(200);

  // formatFile(); // Apenas a primeira vez que gravar o código

  Serial.println("abrir arquivo");

  openFS();


  /*writeFile(str , "/logsAula.txt");

  Serial.println("ler arquivo");

  String test = readFile("/logsAula.txt");*/

  pinMode(buttonPin, INPUT);

  pinMode(ledPin, OUTPUT);


  ledState = readFile("/ledLogs.txt");

  if (ledState == ""){
    ledState = "0";
  }

  digitalWrite(ledPin, ledState.toInt());

}



void loop() {
    nowTime = ntp.getEpochTime(); // Obtém o tempo em segundos desde 1 de janeiro de 1970

  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW){

    if(ledState == "1"){

      ledState = "0";

    } else {

      ledState = "1";

    }

    digitalWrite(ledPin, ledState.toInt());

    hora = ntp.getFormattedTime();
    Serial.println("Hora da gravação: ");
    Serial.println(hora);     /* Escreve a hora no monitor serial. */
    writeFile(ledState, "/ledLogs.txt", hora);

  }

  delay(1500);

  if(nowTime - oldTime >= 1000) {
    Serial.println(readFile("/ledLogs.txt"));
    oldTime = nowTime;
  }

}



void writeFile(String state, String path, String hora) { //escreve conteúdo em um arquivo

  File rFile = SPIFFS.open(path, "w");//a para anexar

  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }

  else {
    Serial.print("tamanho");

    Serial.println(rFile.size());

    rFile.print(state);
    rFile.print("-");
    rFile.println(hora);

    Serial.print("Gravou: ");

    Serial.println(state);
  }

  rFile.close();

}



String readFile(String path) {
  Serial.println("Read file");

  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita

  if(!rFile){
    Serial.println("Erro ao abrir arquivo!");
    s = "";
    return s;
  }

  else {
    Serial.print("----------Lendo arquivo ");

    Serial.print(path);

    Serial.println("  ---------");

    while (rFile.position() < rFile.size()) {

      s = rFile.readStringUntil('\n');

      s.trim();

      Serial.println(s);
    }

    rFile.close();

    return s;
  }
}



void formatFile() {
  Serial.println("Formantando SPIFFS");

  SPIFFS.format();

  Serial.println("Formatou SPIFFS");
}



void openFS(void) {
  if (!SPIFFS.begin()) {
    Serial.println("\nErro ao abrir o sistema de arquivos");
  }

  else {
    Serial.println("\nSistema de arquivos aberto com sucesso!");
  }
}
