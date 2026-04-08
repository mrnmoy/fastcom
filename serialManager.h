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
  Q_PROPERTY(QStringList openModes READ getOpenModes);
  Q_PROPERTY(
      QStringList availablePorts READ getAvailablePorts NOTIFY statusChanged);
  Q_PROPERTY(QStringList availableBaudRates READ getAvailableBaudRates);
  Q_PROPERTY(bool isReadOnly READ getIsReadOnly NOTIFY openModeChanged);

public:
  SerialManager();

signals:
  void statusChanged(bool status);
  void openModeChanged(bool isReadOnly);
  void received(char data);
  void receivedLn();
  void error(QString err);

public slots:
  void open(QString portName, qint64 baudRate, QString openMode);
  void close();
  qint64 send(QString msg);

private slots:
  void opened();
  void closed();
  void readyRead();

private:
  QSerialPort *serialPort;
  // QSerialPortInfo *serialPortInfo;

  bool status, readOnly;

  bool getStatus();
  bool getIsReadOnly();
  QStringList getOpenModes();
  QStringList getAvailablePorts();
  QStringList getAvailableBaudRates();
};

#endif // SERIAL_H
