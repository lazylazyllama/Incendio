#ifndef LUMOS_STORAGEFS_H
#define LUMOS_STORAGEFS_H

#ifdef ESP32
  #include <SPIFFS.h>
#elif defined(ESP8266)
  #include <LittleFS.h>
  #define FILE_READ       "r"
  #define FILE_WRITE      "w"
  #define FILE_APPEND     "a"
#endif

namespace Lumos {
  class StorageFSImpl {
    public:
      bool begin(void);

      bool format(void);

      File open(const char* path, const char* mode);
      File open(const String& path, const char* mode);

      bool exists(const char* path);
      bool exists(const String& path);

      bool remove(const char* path);
      bool remove(const String& path);

      bool rmdir(const char* path);
      bool rmdir(const String& path);

      bool mkdir(const char* path);
      bool mkdir(const String& path);

      bool rename(const char* pathFrom, const char* pathTo);
      bool rename(const String& pathFrom, const String& pathTo);

      void end(void);
  };
}

extern Lumos::StorageFSImpl StorageFS;

#endif /* LUMOS_STORAGEFS_H */
