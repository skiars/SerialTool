#ifndef __VERSION_H
#define __VERSION_H

#define MAIN_VERSION        1.5.7

#define SOFTWARE_NAME       "SerialTool"
#define COPYRIGHT           "Copyleft 2017-2018, Wenliang Guan"

#define _STR_(s)            #s
#define __STR(s)            _STR_(s)

#define BUILD_VERSION       _STR_(87564M)
#define SOFTWARE_VERSION    __STR(MAIN_VERSION)

#if QT_NO_DEBUG
#define SOFTWARE_TITLE      (SOFTWARE_NAME " V" SOFTWARE_VERSION)
#else
#define SOFTWARE_TITLE      (SOFTWARE_NAME " V" SOFTWARE_VERSION \
                            " (Build " BUILD_VERSION "-Debug)")
#endif

#endif
