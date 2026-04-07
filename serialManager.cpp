#include "serialManager.h"

SerialManager::SerialManager() : QObject() {
  status = false;
  serialPort = new QSerialPort();
  // availablePorts = serialPortInfo->availablePorts();
  // availablePorts = QSerialPortInfo::availablePorts();

  for (QSerialPortInfo port : QSerialPortInfo::availablePorts()) {
    availablePortNames.append(port.portName());
    qDebug() << port.portName() << "\n";
  }
}

bool SerialManager::getStatus() { return status; }

void SerialManager::open(QString portName, QSerialPort::BaudRate baudRate,
                         QSerialPort::Direction direction) {
  serialPort->setPortName("/dev/ttyUSB0");
  serialPort->setBaudRate(QSerialPort::Baud115200);

  if (!serialPort->open(QIODevice::ReadWrite)) {
    emit error("Failed to open " + portName + ", " + serialPort->errorString());
    return;
  }

  QObject::connect(serialPort, &QSerialPort::readyRead, this,
                   &SerialManager::readyRead);

  status = true;
  emit statusChanged(status);
}

void SerialManager::close() {
  QObject::disconnect(serialPort, &QSerialPort::readyRead, 0, 0);

  if (serialPort->isOpen())
    serialPort->close();

  status = false;
  emit statusChanged(status);
}

qint64 SerialManager::send(QString msg) {}

void SerialManager::opened() {}
void SerialManager::closed() {}

void SerialManager::readyRead() {
  if (serialPort->isOpen()) {
    // const qint64 BUF_SIZE = 1024;
    // char buf[BUF_SIZE];
    QByteArray buf = serialPort->readAll();
    qDebug() << "Data received:";
    for (qint64 i = 0; i < buf.size(); i++) {
      if (buf[i] == '\n')
        emit receivedLn();
      qDebug() << buf[i];
      emit received(buf[i]);
    }
    // Process the received data...
  }
}
