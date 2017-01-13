

#ifndef PORR_UTILS_H_H
#define PORR_UTILS_H_H


#include <string>
#include <map>

namespace LOGGING {

    enum LogginLevel {

        debug = 0, info, warning, error,
    };

    map<std::string, LogginLevel> StrToLoggingLevel = {
            {"debug", debug},
            {"info", info},
            {"warning", warning},
            {"error", error}
    };

    class LogStream {
    public:

        std::ostream *_dest;
        LogginLevel _level;
        LogginLevel _currentLevel;

        LogStream(std::ostream *dest, LogginLevel level) : _dest(dest), _level(level),
                                                           _currentLevel(level) { }


        static LogStream global(LogginLevel level) {
            static LogStream logStream(&std::cout, level);
            return logStream;
        }

        template<typename T>
        LogStream &operator<<(T const &obj) {
            if (_currentLevel < _level) {
                return *this;
            }

            if (_dest != nullptr) {
                *_dest << obj;
            }
            return *this;
        }


        LogStream &operator<<(const char *obj) {
            if (_currentLevel < _level) {
                return *this;
            }

            if (_dest != nullptr) {
                *_dest << obj;
            }
            return *this;
        }


        LogStream &DEBUG() {
            _currentLevel = debug;
            return *this;
        }

        LogStream &INFO() {
            _currentLevel = info;
            return *this;
        }


        LogStream &W() {
            _currentLevel = warning;
            return *this;
        }


        LogStream &E() {
            _currentLevel = error;
            return *this;
        }

        // function that takes a custom stream, and returns it
        typedef LogStream& (*sf)(LogStream&);

        // take in a function with the custom signature
        LogStream& operator<<(sf manip)
        {
            // call the function, and return it's value
            return manip(*this);
        }


    };

    LogStream& endl(LogStream& logStream) {
        logStream << "\n";
        return logStream;
    }

}
#endif