#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>
#include "HT_SSD1306Wire.h"
#include "LoRaWan_APP.h"
#include "Arduino.h"

#define RF_FREQUENCY 915000000  // Hz
#define LORA_BANDWIDTH 0        // 125 kHz
#define LORA_SPREADING_FACTOR 12
#define LORA_CODINGRATE 1
#define LORA_PREAMBLE_LENGTH 8
#define LORA_SYMBOL_TIMEOUT 0
#define LORA_FIX_LENGTH_PAYLOAD_ON false
#define LORA_IQ_INVERSION_ON false

#define RX_TIMEOUT_VALUE 1000
#define BUFFER_SIZE 30 

const char* ssid = "ssid";
const char* password = "password";

// Configuração do Broker MQTT
const char* mqtt_server = "tb.fse.lappis.rocks";
const char* mqtt_user = "4qNqeNBX0PwPEeyratZv";
const char* mqtt_password = "";  
const char* mqtt_topic = "v1/devices/me/telemetry";

WiFiClient espClient;
PubSubClient client(espClient);

char rxpacket[BUFFER_SIZE];
bool lora_idle = true;
int16_t rssi, rxSize;

static SSD1306Wire display(0x3c, 500000, SDA_OLED, SCL_OLED, GEOMETRY_128_64, RST_OLED);
static RadioEvents_t RadioEvents;

void showMessage(String message, int delayMs = 2000) {
    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 32, message);
    display.display();
    delay(delayMs);
}

// Ativa energia do display OLED
void VextON(void) {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, LOW);
}

// Desativa energia do display OLED
void VextOFF(void) {
    pinMode(Vext, OUTPUT);
    digitalWrite(Vext, HIGH);
}

// Inicializa Wi-Fi
void setup_wifi() {
    Serial.print("Conectando ao Wi-Fi...");
    WiFi.begin(ssid, password);
    if (WiFi.status() != WL_CONNECTED) {
        delay(500);
        showMessage("Erro ao conectar no wifi...");

        Serial.print(".");
    }
    showMessage("Conectou Wifi...");
    Serial.println("\nWiFi conectado!");
}

// Reconectar ao MQTT se necessário
void reconnect_mqtt() {
  
    while (!client.connected()) {
        showMessage("Conectando ao MQTT...");
        if (client.connect("ESP32_Client", mqtt_user, mqtt_password)) {
            showMessage("Conectado ao MQTT!");
        } else {
            showMessage("Falha. Código: ");
            Serial.println(client.state());
            delay(5000);
        }
    }
}

void mqtt_envia_mensagem(const char* topic, const char* mensagem) {
    if (!client.connected()) {
        reconnect_mqtt();
    }
    client.publish(topic, mensagem);
}

void setup() {
    Serial.begin(115200);
    VextON();
    delay(100);

    client.setServer(mqtt_server, 1883);

    display.init();
    display.clear();
    display.display();
    display.setContrast(255);
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_16);
    setup_wifi();

    showMessage("Inicializando RX...");

    showMessage("Inicializando LoRa...");
    SPI.begin();

    showMessage("Config. Eventos LoRa");
    RadioEvents.RxDone = OnRxDone;
    showMessage("Eventos Configurados!");

    showMessage("Radio.Init()");
    Radio.Init(&RadioEvents);
    showMessage("Setando Canal");
    Radio.SetChannel(RF_FREQUENCY);

    showMessage("Setando RX Config");
    Radio.SetRxConfig(MODEM_LORA, LORA_BANDWIDTH, LORA_SPREADING_FACTOR,
                      LORA_CODINGRATE, 0, LORA_PREAMBLE_LENGTH,
                      LORA_SYMBOL_TIMEOUT, LORA_FIX_LENGTH_PAYLOAD_ON,
                      0, true, 0, 0, LORA_IQ_INVERSION_ON, true);
    showMessage("RX Pronto!");
}

void loop() {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Wi-Fi desconectado! Tentando reconectar...");
        WiFi.disconnect();
        WiFi.reconnect();
        int tentativas = 0;

        while (WiFi.status() != WL_CONNECTED && tentativas < 10) { 
            delay(1000);
            Serial.print(".");
            tentativas++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\nWi-Fi reconectado!");
            showMessage("Wi-Fi reconectado!");
        } else {
            Serial.println("\nFalha ao reconectar o Wi-Fi.");
            showMessage("Falha Wi-Fi");
        }
    }
    if (!client.connected()) {
        reconnect_mqtt();
    }

    client.loop();

    if (lora_idle) {
        lora_idle = false;
        showMessage("Aguardando RX...");
        Radio.Rx(0); // Modo contínuo de recepção
    }

    Radio.IrqProcess();
}

void OnRxDone(uint8_t *payload, uint16_t size, int16_t packet_rssi, int8_t snr) {
    rxSize = size;
    rssi = packet_rssi;
    memcpy(rxpacket, payload, size);
    rxpacket[size] = '\0'; // Garante que a string termina corretamente

    Radio.Sleep();
    lora_idle = true;

    display.clear();
    display.setTextAlignment(TEXT_ALIGN_CENTER);
    display.setFont(ArialMT_Plain_10);
    display.drawString(64, 16, "Pacote RX!");
    display.drawString(64, 32, rxpacket); // Mostra o conteúdo recebido
    display.drawString(64, 48, "RSSI: " + String(rssi)); // Mostra a potência do sinal
    display.display();
    delay(3000);

    // **Envia a mensagem via MQTT**
    char mensagem[100];
    sprintf(mensagem, "{\"umidade\": %s}", rxpacket);
    showMessage(mensagem);
    mqtt_envia_mensagem(mqtt_topic, mensagem);

    // showMessage("Mensagem enviada por mqtt!");
}
