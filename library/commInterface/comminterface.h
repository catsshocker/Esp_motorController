#ifndef COMM_INTERFACE_H
#define COMM_INTERFACE_H

#include "Arduino.h"
#include "driver/twai.h"

class CommInterface {
public:
    virtual ~CommInterface() = default;
    virtual void sendData(const char* data, size_t size, uint32_t dataID) = 0;
    virtual int receiveData(char* buffer, size_t bufferSize) = 0;
    virtual bool isReady() const = 0;
};

class UART : public CommInterface {
private:
    int rxPin;
    int txPin;
    int baudRate;
public:
    UART(int rxPin = -1, int txPin = -1, int baudRate = 115200);
    ~UART() override = default;

    void sendData(const char* data, size_t size, uint32_t dataID) override;
    int receiveData(char* buffer, size_t bufferSize) override;
    bool isReady() const override;
};

class Can : public CommInterface {
private:
    int rxPin;
    int txPin;
    int baudRate;
public:
    Can(int rxPin = -1, int txPin = -1, int baudRate = 500000);
    ~Can() override = default;

    void sendData(const char* data, size_t size, uint32_t dataID) override;
    int receiveData(char* buffer, size_t bufferSize) override;
    bool isReady() const override;
};

#endif
