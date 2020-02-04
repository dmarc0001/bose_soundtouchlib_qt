#ifndef MAIN_HPP
#define MAIN_HPP

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <memory>
#include "commserver/BoseCommServer.hpp"
#include "config/DaemonConfig.hpp"
#include "config/programconfig.hpp"

int main( int argc, char *argv[] );

#endif  // MAIN_HPP
