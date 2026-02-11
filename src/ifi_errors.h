#ifndef IFI_ERRORS_H
#define IFI_ERRORS_H

typedef enum {
    IFI_OK = 0,           // Ok
    IFI_ERROR_OPEN,       // Could not open file
    IFI_ERROR_READ,       // Read failed (EOF or corrupt)
    IFI_ERROR_SIGNATURE,  // Magic "IWAD" or "PWAD" not found
    IFI_ERROR_MEMORY,     // malloc failed
    IFI_ERROR_NULL_ARGS,  // Arguments were null
} ifi_error_t;

#endif
