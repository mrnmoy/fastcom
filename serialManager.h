#ifndef SERIAL_MANAGER_H
#define SERIAL_MANAGER_H

#include <QDebug>
#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>

class SerialManager : public QObject {
  Q_OBJECT

  Q_PROPERTY(bool isConnected READ status NOTIFY statusChanged);
  Q_PROPERTY(QStringList openModes READ openModes);
  Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY
                 availablePortsChanged);
  Q_PROPERTY(QStringList availableBaudRates READ availableBaudRates);
  Q_PROPERTY(bool isReadOnly READ isReadOnly NOTIFY openModeChanged);

public:
  SerialManager();

signals:
  void statusChanged(bool status);
  void availablePortsChanged();
  void openModeChanged(bool isReadOnly);
  void received(QString data);
  void receivedLn();
  void error(QString err);

public slots:
  void open(QString portName, qint64 baudRate, QString openMode);
  void close();
  void send(const QByteArray &data);

private slots:
  void opened();
  void closed();
  void readyRead();

private:
  QSerialPort *serialPort;

  bool _status, _readOnly, _pendingLn;

  bool status();
  bool isReadOnly();
  QStringList openModes();
  QStringList availablePorts();
  QStringList availableBaudRates();
};

#endif
