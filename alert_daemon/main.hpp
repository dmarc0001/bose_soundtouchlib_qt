#ifndef MAIN_HPP
#define MAIN_HPP

#include <signal.h>

#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <memory>

#include "config/alertconfig.hpp"
#include "daemon/bosecommserver.hpp"

int main( int argc, char *argv[] );
static int setup_unix_signal_handlers();

#endif  // MAIN_HPP
