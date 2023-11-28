# praticaSPIFFS
<h1 id="usage" > 💻 Descrição </h1>

Esse código é responsavel por ler a entrada de um botão e ascender ou apagar um LED,
mas com o diferencial que o estado do LED é armazenado em um arquivo interno do esp 32
utilizando SPIFFS, que guarda uma quantidade maxima definida no código. Quando o Esp32
é ligado, ele checa o ultimo valor do LED e já inicia o LED com o estado correspondente ao log.

<h1 id="usage" > 📚 Bibliotecas </h1>
- FS.h<br>
- SPIFFS.h<br>
- NTPClient.h<br>
- WiFi.h<br>
- queue

<h1 id="usage" > 🧰 Componentes utilizados </h1>
- Esp32<br>
- Protoboard<br>
- Botão<br>
- LED<br>
- Dois resistores de 1K ohm<br>
- Jumpers

<h3>Autor</h3>
<a href="https://github.com/JoabUrbano">Joab Urbano</a><br>
