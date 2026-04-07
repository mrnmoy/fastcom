#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QStringListModel>

class SerialManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isConnected READ getStatus NOTIFY statusChanged);

public:
  SerialManager();
  QList<QSerialPortInfo> availablePorts = QSerialPortInfo::availablePorts();
  QStringListModel *availablePortNames;
  QStringList availableBaudRates = {"1200",  "2400",  "4800",  "9600",
                                    "19200", "38400", "57600", "115200"};

signals:
  // void availablePorts();
  void statusChanged(bool status);
  void received(char data);
  void receivedLn();
  void error(QString err);

public slots:
  void open(QString portName, qint64 baudRate);
  void close();
  qint64 send(QString msg);

private slots:
  void opened();
  void closed();
  void readyRead();

private:
  QSerialPort *serialPort;
  // QSerialPortInfo *serialPortInfo;

  bool status;

  bool getStatus();
};

#endif // SERIAL_H
