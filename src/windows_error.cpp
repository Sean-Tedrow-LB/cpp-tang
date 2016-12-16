#ifdef _WIN32
#include "windows_error.hpp"
#include <Windows.h>
#include "ix_unicode.hpp"

std::string windows_error_string()
{
    std::u16string message;
    message.resize(1023);
    char16_t *message_p = &(message[0]);
    DWORD length16 = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(), 0, 
                                   (wchar_t*)message_p, 1023, nullptr);
	int length8 = ix_measure_utf16_to_utf8(message_p, length16);
    std::string string_out;
    string_out.resize(length8);
    ix_convert_utf16_to_utf8(&(string_out[0]), message_p, length16);
    return string_out;
}

#endif