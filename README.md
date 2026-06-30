# Custom SDL2 UI Framework in C 🖥️

Ein modulares, leichtgewichtiges und objektorientiertes GUI-Framework, das komplett in C geschrieben ist und auf der **SDL2-Bibliothek** basiert. Es ermöglicht das einfache Erstellen, Verwalten und Rendern von interaktiven UI-Elementen wie Buttons, Slidern und Toggles.

## 🚀 Features

* **Objektorientierter Ansatz in C**: Nutzung von Struktur-Komposition (`UIElement base`) zur Vererbung von Eigenschaften an spezifische Widgets.
* **Effiziente Statusverwaltung**: Einsatz von C-Bitfields (`UIFlags`), um Sichtbarkeit, Hover- und Klick-Zustände extrem speicherschonend (1 Byte) zu verwalten.
* **Event-gesteuertes Callback-System**: Dynamische Zuweisung von Funktionszeigern, sodass jedes UI-Element bei Klicks oder Bewegungen eigene Logik (`buttonCallback`, `sliderCallback`) ausführen kann.
* **Dynamisches Screen-Management**: Flexibles Hinzufügen von UI-Elementen zur Laufzeit über ein dynamisches Array (`realloc`) inklusive sauberer Speicherbereinigung (`freeScreen`).

## 📁 Dateistruktur

* **`extension.h`**: Das Herzstück des Frameworks. Definiert die Basis-Strukturen (`UIElement`, `UIFlags`), die Widget-Typen (`Button`, `Toggle`, `Slider`) und den Layout-Container (`Screen`).
* **`extension.c`**: Implementiert das Event-Handling (Mausklicks, Bewegungen), das Rendern der Elemente und die Speicherverwaltung des Screens.
* **`main1.c`**: Anwendungsbeispiel. Initialisiert das SDL2-Fenster, erstellt einen Screen, fügt interaktive Elemente hinzu und verknüpft sie mit Callback-Funktionen.

## 🛠️ Architektur & Code-Beispiele

### Widgets per Struktur-Komposition
Jedes Steuerelement bettet das `UIElement` als Basis ein, wodurch Vererbung in C simuliert wird:

```c
typedef struct {
    UIElement base;      // Enthält Position, Größe, Flags und Event-Funktionen
    Color color_on;
    Color color_off;
    void (*onclick)(void*);
    void *param;
} Button;
