#include <FS.h>
#include "SPIFFS.h"



int buttonPin = 21;
int ledPin = 22;



String ledState;
String str;
String s;

int num = 0;



void writeFile(String state, String path) { //escreve conteúdo em um arquivo

  File rFile = SPIFFS.open(path, "w");//a para anexar

  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
  }

  else {
    Serial.print("tamanho");

    Serial.println(rFile.size());

    rFile.println(state);

    Serial.print("Gravou: ");

    Serial.println(state);
  }

  rFile.close();

}



String readFile(String path) {
  Serial.println("Read file");

  File rFile = SPIFFS.open(path, "r");//r+ leitura e escrita

  if(rFile == NULL){
    s = "";
    return s;
  }

  if (!rFile) {
    Serial.println("Erro ao abrir arquivo!");
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



void setup() {

  Serial.begin(115200);
  Serial.println("inicio");

  delay(200);

  // formatFile(); // Apenas a primeira vez que gravar o código

  Serial.println("abrir arquivo");

  openFS();

  /*num = random(5000);//gerando numero aleatorio para salvar no arquivo

  str = String(num);//convertendo o int para String

  writeFile(str , "/logsAula.txt");

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

  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW){

    if(ledState == "1"){

      ledState = "0";

    } else {

      ledState = "1";

    }

    digitalWrite(ledPin, ledState.toInt());

    writeFile(ledState, "/ledLogs.txt");

  }

  delay(1500);

  Serial.println(readFile("/ledLogs.txt"));

}
