#include "soundtouchdiscover.hpp"
#include <QCoreApplication>

int main(int argc, char *argv[]) {
  QCoreApplication a(argc, argv);
  SoundTouchDiscover discover;
  discover.run();
  return a.exec();
}
