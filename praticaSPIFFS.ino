#include <FS.h>
#include "SPIFFS.h"
#include <NTPClient.h> // https://github.com/arduino-libraries/NTPClient
#include <WiFi.h>

#define wifi_ssid "NPITI-IoT"
#define wifi_password "NPITI-IoT"

WiFiUDP udp;
NTPClient ntp(udp, "a.st1.ntp.br", -3 * 3600, 60000); // Cria um objeto "NTP" com as configurações.utilizada no Brasil
String hora;

int nowTime, oldTime = 0, num = 0;

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

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  digitalWrite(LED_BUILTIN, HIGH); 

  ntp.begin(); // Inicia o protocolo
  ntp.forceUpdate(); // Atualização

  delay(200);

  // formatFile(); // Apenas a primeira vez que gravar o código

  Serial.println("Abrir arquivo");
  openFS();
  ledState = readFile("/ledLogs.txt");

  if (ledState == "") ledState = "0";

  digitalWrite(ledPin, ledState.toInt());
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

    Serial.println("Hora da gravação: ");

    Serial.println(hora); // Escreve a hora no monitor serial.

    writeFile(ledState, "/ledLogs.txt", hora);
  }

  if (nowTime - oldTime >= 20)
  { // Ler o arquivo a cada 20 segundos

    Serial.println(readFile("/ledLogs.txt"));

    oldTime = nowTime;
  }
}

void writeFile(String state, String path, String hora)
{ // escreve conteúdo em um arquivo

  File rFile = SPIFFS.open(path, "w"); // a para anexar

  if (!rFile)
    Serial.println("Erro ao abrir arquivo!");

  else
  {

    rFile.print(state);
    rFile.print("-");
    rFile.println(hora);
    Serial.print("Gravou: ");

    Serial.println(state);
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
    Serial.print("---------- Lendo arquivo ");

    Serial.print(path);

    Serial.println("  ---------");

    while (rFile.position() < rFile.size())
    {

      s = rFile.readStringUntil('\n');

      s.trim();

      Serial.println(s);
    }

    rFile.close();

    return s;
  }
}

void formatFile()
{

  Serial.println("Formantando SPIFFS");

  SPIFFS.format();

  Serial.println("Formatou SPIFFS");
}

void openFS(void)
{

  if (!SPIFFS.begin())
    Serial.println("\nErro ao abrir o sistema de arquivos");

  else
    Serial.println("\nSistema de arquivos aberto com sucesso!");
}
