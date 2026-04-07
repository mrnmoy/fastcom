#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

class SerialManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isConnected READ getStatus NOTIFY statusChanged);

public:
  SerialManager();

signals:
  void statusChanged(bool);
  void received(QByteArray data);
  void error(QString err);

public slots:
  void open(QString portName,
            QSerialPort::BaudRate baudRate = QSerialPort::Baud115200,
            QSerialPort::Direction direction = QSerialPort::AllDirections);
  void close();
  qint64 send(QString msg);

private slots:
  void opened();
  void closed();
  void readyRead();

private:
  QSerialPort *serialPort;

  bool status;

  bool getStatus();
};

#endif // SERIAL_H
