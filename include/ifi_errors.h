#ifndef IFI_ERRORS_H
#define IFI_ERRORS_H

typedef enum {
    IFI_OK = 0,          // Ok
    IFI_ERROR_OPEN,      // Could not open file
    IFI_ERROR_READ,      // Read failed (EOF or corrupt)
    IFI_ERROR_SIGNATURE, // Magic "IWAD" or "PWAD" not found
    IFI_ERROR_MEMORY,    // malloc failed
    IFI_ERROR_NULL_ARGS, // Arguments were null
    IFI_ERROR_ENCODING,  // Encoding error, e.g. snprintf negative numbers
    IFI_ERROR_WRITE,     // Write error, e.g. printf negative numbers
    IFI_ERROR_INVALID,   // Invalid state of object being constructed
    IFI_ERROR_SEEK,      // File pointer seek failed
} ifi_error_t;

#endif
