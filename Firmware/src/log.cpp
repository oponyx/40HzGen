#include "log.h"

size_t m_log(bool pr_tstamp, const char *format, ...){
    va_list arg;
    va_start(arg, format);
    char temp[64];
    char* buffer = temp;
    size_t len = vsnprintf(temp, sizeof(temp), format, arg);
    va_end(arg);
    if (len > sizeof(temp) - 1) {
        buffer = new char[len + 1];
        if (!buffer) {
            return 0;
        }
        va_start(arg, format);
        vsnprintf(buffer, len + 1, format, arg);
        va_end(arg);
    }
    len = (pr_tstamp) ? Serial.printf("[%lu] %s",millis(), buffer) : Serial.printf("%s", buffer);
    if (buffer != temp) {
        delete[] buffer;
    }
    return len;
}
