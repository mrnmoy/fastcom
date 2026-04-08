#include "serialManager.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  qmlRegisterType<SerialManager>("SerialManager", 1, 0, "SerialManager");

  QQmlApplicationEngine engine;
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("fastcom", "Main");

  // return QCoreApplication::exec();
  return app.exec();
}
