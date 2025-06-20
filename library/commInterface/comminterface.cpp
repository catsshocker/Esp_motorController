#include "commInterface.h"

UART::UART(int rxPin, int txPin, int baudRate)
    : rxPin(rxPin), txPin(txPin), baudRate(baudRate) {
    if (rxPin < 0 || txPin < 0) {
        return;
    }
    Serial.begin(baudRate, SERIAL_8N1, rxPin, txPin);
}

UART::~UART() {
    Serial.end();
}

void UART::sendData(const char* data, size_t size, uint32_t dataID) {
    if (data == nullptr || size == 0) {
        return;
    }
    Serial.write(reinterpret_cast<const uint8_t*>(&dataID), 4); // 傳送 dataID 4 bytes
    Serial.write((const uint8_t*)data, size);
}

int UART::receiveData(char* buffer, size_t bufferSize) {
    if (buffer == nullptr || bufferSize == 0) {
        return 0;
    }
    int bytesRead = Serial.readBytes(buffer, bufferSize);
    return bytesRead;
}

bool UART::isReady() const {
    return Serial.availableForWrite() > 0;
}

Can::Can(int rxPin, int txPin, int baudRate)
    : rxPin(rxPin), txPin(txPin), baudRate(baudRate) {
    if (rxPin < 0 || txPin < 0) {
        return;
    }
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT((gpio_num_t)rxPin, (gpio_num_t)txPin, TWAI_MODE_NORMAL);
    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_500KBITS();
    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();
    twai_driver_install(&g_config, &t_config, &f_config);
    twai_start();
}

Can::~Can() {
    twai_stop();
    twai_driver_uninstall();
}

void Can::sendData(const char* data, size_t size, uint32_t dataID) {
    if (data == nullptr || size == 0) {
        return;
    }
    twai_message_t message = {};
    message.identifier = dataID;
    message.extd = false; // 標準格式
    message.data_length_code = size > 8 ? 8 : size; // CAN 最大 8 bytes
    memcpy(message.data, data, message.data_length_code);

    twai_transmit(&message, portMAX_DELAY);
}

int Can::receiveData(char* buffer, size_t bufferSize) {
    if (buffer == nullptr || bufferSize == 0) {
        return 0;
    }
    twai_message_t message;
    if (twai_receive(&message, portMAX_DELAY) == ESP_OK) {
        if (message.data_length_code > bufferSize) {
            return -1; // Buffer 太小
        }
        memcpy(buffer, message.data, message.data_length_code);
        return message.data_length_code;
    }
    return 0; // 沒收到訊息
}

bool Can::isReady() const {
    twai_status_info_t status;
    twai_get_status_info(&status);
    return status.state == TWAI_STATE_RUNNING;
}

