#include "serialManager.h"

SerialManager::SerialManager() : QObject() {
  status = false;
  readOnly = true;
  serialPort = new QSerialPort();
}

bool SerialManager::getStatus() { return status; }

bool SerialManager::getIsReadOnly() { return readOnly; }

QStringList SerialManager::getOpenModes() {
  return (QStringList() << "ReadOnly" << "WriteOnly" << "ReadWrite");
}

QStringList SerialManager::getAvailablePorts() {
  QStringList portNames;
  for (QSerialPortInfo portInfo : QSerialPortInfo::availablePorts()) {
    portNames << portInfo.portName();
  }
  return portNames;
}

QStringList SerialManager::getAvailableBaudRates() {
  return (QStringList() << "1200" << "2400" << "4800" << "9600" << "19200"
                        << "38400" << "57600" << "115200");
}

void SerialManager::open(QString portName, qint64 baudRate, QString openMode) {
  serialPort->setPortName(portName);
  serialPort->setBaudRate(baudRate);

  QIODeviceBase::OpenMode _openMode;

  if (openMode == "ReadWrite") {
    _openMode = QIODevice::ReadWrite;
    readOnly = false;
  } else if (openMode == "WriteOnly") {
    _openMode = QIODevice::WriteOnly;
    readOnly = false;
  } else {
    _openMode = QIODevice::ReadOnly;
    readOnly = true;
  }

  emit openModeChanged(readOnly);

  if (!serialPort->open(_openMode)) {
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
