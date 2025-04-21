# EOS Framework

**EOS Framework** es un framework embebido para ESP32/ESP8266 que permite
definir periféricos con un pequeño DSL en C++ y generar automáticamente
el boilerplate con un Clang Plugin.

## Instalación

En un proyecto PlatformIO:

```ini
[env:esp32dev]
platform = espressif32
framework = arduino
board = esp32dev

lib_deps =
  tuOrg/eos-framework

build_flags =
  -Igenerated
  -Iinclude

extra_scripts =
  lib/eos-framework/scripts/prebuild.py
```