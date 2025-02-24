# HydroSense

HydroSense é um projeto que visa otimizar o uso da água na irrigação, coletando dados sobre umidade do solo e enviando para um dashboard em nuvem.

## Requisitos

Antes de iniciar, certifique-se de que possui os seguintes componentes instalados:

### Hardware:
- Heltec WiFi LoRa 32
- Sensor Capacitivo de Umidade do Solo V2 ([link](https://www.eletrogate.com/sensor-capacitivo-de-umidade-do-solo-v20-com-regulador?utm_source=Site&utm_medium=GoogleMerchant&utm_campaign=GoogleMerchant&utm_source=google&utm_medium=cpc&utm_campaign=[MC4]_[G]_[PMax]_Categorias&utm_content=&utm_term=&gad_source=1&gclid=CjwKCAiAzvC9BhADEiwAEhtlN8NtPbijGBWjFcwPkwnn13-I6cQKkhhd4EW3xzI3SEYRbHvmPfw9HxoC2s0QAvD_BwE))
- Módulo LoRa (embutido na Heltec WiFi LoRa 32)

### Software:
- Arduino IDE

## Instalação

### Configuração do Ambiente
1. Instale a **Arduino IDE** no seu sistema: [Download Arduino IDE](https://www.arduino.cc/en/software)
2. Adicione o suporte à Heltec WiFi LoRa 32 na Arduino IDE:
   - No menu **Arquivo** -> **Preferências**, adicione o seguinte link na opção "URLs adicionais para Gerenciadores de Placas":
     ```
     https://resource.heltec.cn/download/package_heltec_esp32_index.json
     ```
   - Em seguida, vá em **Ferramentas** -> **Placa** -> **Gerenciador de Placas**, pesquise por `Heltec ESP32` e instale a versão mais recente.

3. Instale as bibliotecas necessárias via Arduino IDE (**Gerenciador de Bibliotecas**):
   - **WiFi** (embutida na Heltec WiFi LoRa 32)
   - **LoRa** (compatível com a Heltec WiFi LoRa 32)
   - **PubSubClient**

### Upload do Código
1. Clone este repositório no seu computador:
   ```sh
   git clone https://github.com/ciceroff/hydro-sense.git
   cd hydro-sense
   ```
2. Abra o código na Arduino IDE.
3. Selecione a placa correta (**Heltec WiFi LoRa 32**) e a porta serial correspondente.
4. Compile e envie cada código para a Heltec WiFi LoRa 32.

## Uso

1. Conecte os sensores à Heltec WiFi LoRa 32, ligando as saídas nos pinos 36 e 37.
2. Ligue a Heltec WiFi LoRa 32 e olhe o display para ver o funcionamento do sistema.

