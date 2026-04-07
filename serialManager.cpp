#include "serialManager.h"

SerialManager::SerialManager() : QObject() {
  status = false;
  serialPort = new QSerialPort();
}

bool SerialManager::getStatus() { return status; }

void SerialManager::open(QString portName, QSerialPort::BaudRate baudRate,
                         QSerialPort::Direction direction) {
  serialPort->setPortName("/dev/ttyUSB0");
  serialPort->setBaudRate(QSerialPort::Baud115200);

  if (!serialPort->open(QIODevice::ReadWrite)) {
    qDebug() << "Failed to open port: " << serialPort->portName()
             << ", Error: " << serialPort->errorString();
  }

  QObject::connect(serialPort, &QSerialPort::readyRead, this,
                   &SerialManager::readyRead);

  status = true;
  emit statusChanged(status);
  qDebug() << "Serial port opened successfully";
}

void SerialManager::close() {
  QObject::disconnect(serialPort, &QSerialPort::readyRead, 0, 0);

  // if (tcpSocket->state())
  //   tcpSocket->disconnectFromHost();
  // else
  //   tcpSocket->abort();
}

qint64 SerialManager::send(QString msg) {}

void SerialManager::opened() {}
void SerialManager::closed() {}

void SerialManager::readyRead() {
  if (serialPort->isOpen()) {
    QByteArray data = serialPort->readAll();
    qDebug() << "Data received:" << data;
    emit received(data);
    // Process the received data...
  }
}
