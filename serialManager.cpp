#include "serialManager.h"

SerialManager::SerialManager() : QObject() {
  status = false;
  serialPort = new QSerialPort();

  // availableBaudRates = {"1200",  "2400",  "4800",  "9600",
  //                       "19200", "38400", "57600", "115200"};
  availableBaudRates =
      (QStringList() << "1200" << "2400" << "4800" << "9600" << "19200"
                     << "38400" << "57600" << "115200");

  // availablePorts = serialPortInfo->availablePorts();
  // availablePorts = QSerialPortInfo::availablePorts();

  availablePortNames = new QStringListModel();
  QStringList portNames;
  // for (int i = 0; i < availablePorts.size(); i++) {
  for (QSerialPortInfo portInfo : QSerialPortInfo::availablePorts()) {
    // portNames.append(port.portName());
    // availablePortNames.append(portInfo.portName());
    qDebug() << portInfo.portName() << "\n";
  }
  availablePortNames->setStringList(portNames);
}

bool SerialManager::getStatus() { return status; }

void SerialManager::open(QString portName, qint64 baudRate) {
  serialPort->setPortName(portName);
  serialPort->setBaudRate(baudRate);

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
