# EOS Framework – Skeleton 0.1

Professional starting point for a DSL‑driven IoT framework that targets
ESP8266 but can be unit‑tested on the host.  Key features:

* 📄 **DSL** (`*.eos`) for declaring peripherals, shared variables,
  services, events, periodic tasks and board‑level wiring.
* ⚙️ **Core runtime** (`eos_core`) – extremely small, header‑only façade
  with static tables; no dynamic allocation, no RTTI.
* 🛠 **prebuild.py** – stub script that will parse DSL files and emit
  `eos_gen_*.cpp` before compilation.
* 🚀 Works with **PlatformIO** for firmware and **CMake** for host tests.

## Build (host)
```bash
mkdir build && cd build
cmake .. && cmake --build .
./host_tests
```

## Build (ESP8266)
```bash
pio run -t upload
```

<!-- # EOS - EspOperationSystem

## Descripción

**EOS-EspOperationSystem** es un sistema operativo modular y flexible diseñado específicamente para dispositivos embebidos como el ESP8266 y ESP32. EOS facilita la interacción entre **periféricos** y **procesos**, proporcionando una arquitectura en capas que permite una comunicación eficiente mediante **eventos**, **servicios** y **almacenamiento compartido**.

El objetivo de EOS es ofrecer una solución robusta y extensible para manejar hardware de manera sencilla, y al mismo tiempo, permitir una lógica avanzada en el manejo de los datos y el control de periféricos.

---

## Arquitectura

### 1. **Capa del Núcleo**
   El núcleo del sistema se encarga de gestionar el ciclo de vida de los periféricos y procesos, así como de manejar aspectos del sistema como Wi-Fi, energía, y almacenamiento persistente.

### 2. **Capa de Periféricos**
   Los periféricos se encargan de manejar dispositivos de entrada/salida (sensores, actuadores) y se ejecutan periódicamente. Generan eventos y exponen servicios para que otros componentes puedan interactuar con ellos.

### 3. **Capa de Procesos**
   Los procesos son responsables de la lógica avanzada del sistema y se alimentan de eventos, servicios, y datos compartidos generados por los periféricos.

---

## Instalación

Para comenzar a trabajar con **EOS-EspOperationSystem**, necesitarás tener **PlatformIO** instalado en tu entorno de desarrollo. Sigue estos pasos para configurar tu entorno:

1. **Instalar PlatformIO**: Si no tienes PlatformIO instalado, puedes encontrar las instrucciones aquí: [PlatformIO Installation](https://platformio.org/install).
   
2. **Clonar el repositorio**:
   ```bash
   git clone https://github.com/<tu_usuario>/EOS-EspOperationSystem.git
   cd EOS-EspOperationSystem
  ``` -->