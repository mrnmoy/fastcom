#include "serialManager.h"

SerialManager::SerialManager() : QObject() {
  _status = false;
  _readOnly = true;
  _pendingLn = true;
  serialPort = new QSerialPort();
}

bool SerialManager::status() {
  _pendingLn = true;
  return _status;
}

bool SerialManager::isReadOnly() { return _readOnly; }

QStringList SerialManager::openModes() {
  return (QStringList() << "ReadWrite" << "ReadOnly" << "WriteOnly");
}

QStringList SerialManager::availablePorts() {
  QStringList portNames;
  for (QSerialPortInfo portInfo : QSerialPortInfo::availablePorts()) {
    portNames << portInfo.portName();
  }
  return portNames;
}

QStringList SerialManager::availableBaudRates() {
  return (QStringList() << "115200" << "57600" << "38400" << "19200" << "9600"
                        << "4800" << "2400" << "1200");
}

void SerialManager::open(QString portName, qint64 baudRate, QString openMode) {
  serialPort->setPortName(portName);
  serialPort->setBaudRate(baudRate);

  QIODeviceBase::OpenMode _openMode;

  if (openMode == "ReadWrite") {
    _openMode = QIODevice::ReadWrite;
    _readOnly = false;
  } else if (openMode == "WriteOnly") {
    _openMode = QIODevice::WriteOnly;
    _readOnly = false;
  } else {
    _openMode = QIODevice::ReadOnly;
    _readOnly = true;
  }

  emit openModeChanged(_readOnly);

  if (!serialPort->open(_openMode)) {
    emit error("Failed to open " + portName + ", " + serialPort->errorString());
    return;
  }

  QObject::connect(serialPort, &QSerialPort::readyRead, this,
                   &SerialManager::readyRead);

  _status = true;
  emit statusChanged(_status);
}

void SerialManager::close() {
  QObject::disconnect(serialPort, &QSerialPort::readyRead, 0, 0);

  if (serialPort->isOpen())
    serialPort->close();

  _status = false;
  emit statusChanged(_status);
}

void SerialManager::opened() {}
void SerialManager::closed() {}

void SerialManager::readyRead() {
  if (serialPort->isOpen()) {
    const QByteArray buf = serialPort->readAll();
    qDebug() << "Data received:";
    for (qint64 i = 0; i < buf.size(); i++) {
      if (_pendingLn) {
        emit receivedLn();
        _pendingLn = false;
      }
      if (buf.at(i) == '\n') {
        if (buf.size() == i + 1) {
          _pendingLn = true;
        } else {
          emit receivedLn();
          _pendingLn = false;
        }
      } else {
        emit received(QString(buf.at(i)));
      }
    }
  }
}

void SerialManager::send(const QByteArray &data) {
  if (!serialPort->write(data)) {
    emit error("Unable to write");
  }
}
