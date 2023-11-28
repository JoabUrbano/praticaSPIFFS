#include <FS.h>
#include "SPIFFS.h"
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient
#include <WiFi.h>
#include <queue>

std::queue<String> filaDeStrings;

#define wifi_ssid "NPITI-IoT"
#define wifi_password "NPITI-IoT"

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); // Cria um objeto "NTP" com as configurações.utilizada no Brasil
String hora;

int nowTime, oldTime = 0, num = 0;
int maxLines = 10; // Número máximo de linhas a serem mantidas

int buttonPin = 21;
int ledPin = 22;
int LED_BUILTIN = 2;

String ledState, str, s;

/* Declaração de funções */
void writeFile(String state, String path, String hora);
String readFile(String path);
void formatFile();
void openFS(void);

void setup()
{

  Serial.begin(115200);

  Serial.println("inicio");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  digitalWrite(LED_BUILTIN, HIGH);

  ntp.begin();       // Inicia o protocolo
  ntp.forceUpdate(); // Atualização

  delay(200);

  // formatFile(); // Apenas a primeira vez que gravar o código

  Serial.println("Abrir arquivo");
  openFS();

  ledState = readFile("/ledLogs.txt");
  int pos = ledState.indexOf(',');
  String ledS = ledState.substring(0, pos);

  if (ledS == "")
    ledS = "0";

  digitalWrite(ledPin, ledS.toInt());
}

void loop()
{

  nowTime = ntp.getEpochTime(); // Obtém o tempo em segundos desde 1 de janeiro de 1970

  int buttonState = digitalRead(buttonPin);

  if (buttonState == LOW)
  {

    if (ledState == "1")
      ledState = "0";

    else
      ledState = "1";

    delay(220);

    digitalWrite(ledPin, ledState.toInt());

    hora = ntp.getFormattedTime();

    writeFile(ledState, "/ledLogs.txt", hora);
  }

  if (nowTime - oldTime >= 20)
  { // Ler o arquivo a cada 20 segundos

    readFile("/ledLogs.txt");

    oldTime = nowTime;
  }
}

void writeFile(String state, String path, String hora)
{
  while (!filaDeStrings.empty()) // Esvazia a fila se ela tiver algo
  {
    filaDeStrings.pop();
  }

  readFile(path); // Chama o readFile para preencher a fila
  File rFile = SPIFFS.open(path, "w");

  if (!rFile) // Se não abriu retorna
  {
    Serial.println("Erro ao abrir arquivo!");
    return;
  }
  filaDeStrings.push(state + "," + hora); // adiciona o ultimo estado a fila

  while (filaDeStrings.size() > maxLines) // enquanto houver maiss logs que o maximo, remova-os
  {
    filaDeStrings.pop();
  }
  while (!filaDeStrings.empty()) // os logs dentro do limite de tamanho são recolocados no arquivo
  {
    rFile.println(filaDeStrings.front());
    filaDeStrings.pop();
  }

  rFile.close();
}

String readFile(String path)
{
  Serial.println("Read file");

  File rFile = SPIFFS.open(path, "r"); // r+ leitura e escrita

  if (!rFile)
  {
    Serial.println("Erro ao abrir arquivo!");
    s = "";

    return s;
  }

  else
  {
    Serial.print("---------- Lendo arquivo " + path + "  ---------");

    while (rFile.position() < rFile.size())
    {
      String line = rFile.readStringUntil('\n'); // Lê uma linha do arquivo
      s = line; 
      Serial.println(line);
      filaDeStrings.push(line); // preenche a fila se for utilizar a escrita
    }

    rFile.close();
  }
  return s;
}

void formatFile() // Formata o ESP32
{

  Serial.println("Formantando SPIFFS");

  SPIFFS.format();

  Serial.println("Formatou SPIFFS");
}

void openFS(void) // Inicia i Fs
{

  if (!SPIFFS.begin())
    Serial.println("\nErro ao abrir o sistema de arquivos");

  else
    Serial.println("\nSistema de arquivos aberto com sucesso!");
}
