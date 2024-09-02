#ifndef MQTT_H
#define MQTT_H

#include "esp_system.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"
#include "freertos/queue.h"
#include "cJSON.h"
#include "mqtt_client.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

void mqtt_start();

void mqtt_envia_mensagem(char *topico, char *mensagem);

#endif