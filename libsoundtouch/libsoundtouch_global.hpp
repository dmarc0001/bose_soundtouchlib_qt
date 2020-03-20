#ifndef LIBSOUNDTOUCH_GLOBAL_HPP
#define LIBSOUNDTOUCH_GLOBAL_HPP

#include <QtCore/qglobal.h>

#ifdef SOUNDTOUCH_QT_LIB_IMPORT
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_IMPORT
#else
#ifdef SOUNDTOUCH_QT_LIB_EXPORT
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT Q_DECL_EXPORT
#else
#define SOUNDTOUCH_QT_LIBSHARED_EXPORT
#endif
#endif

/*
#if defined(LIBSOUNDTOUCH_LIBRARY)
#  define LIBSOUNDTOUCH_EXPORT Q_DECL_EXPORT
#else
#  define LIBSOUNDTOUCH_EXPORT Q_DECL_IMPORT
#endif
*/
#endif  // LIBSOUNDTOUCH_GLOBAL_HPP
