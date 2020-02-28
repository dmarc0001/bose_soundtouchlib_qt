#ifndef MAIN_HPP
#define MAIN_HPP

#include <signal.h>
#include <QtCore/QCommandLineOption>
#include <QtCore/QCommandLineParser>
#include <QtCore/QCoreApplication>
#include <iostream>
#include <memory>
#include "config/alertconfig.hpp"
#include "daemon/bosecommserver.hpp"
#include "support/nologgerexception.hpp"

typedef void ( *SignalHandlerPointer )( int );
// typedef void ( *CommServerPointer )( *bose_commserver::BoseCommServer::reciveAsyncSignal( int ) );
static SignalHandlerPointer previousINTHandler{nullptr};
static SignalHandlerPointer previousTERMHandler{nullptr};
static SignalHandlerPointer previousHUPHandler{nullptr};
//
// zeiger auf das Serverobjekt
//
static bose_commserver::BoseCommServer *commInstance{nullptr};

int main( int argc, char *argv[] );
static void signalHandler( int signal );

#endif  // MAIN_HPP
