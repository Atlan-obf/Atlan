# AI Assistant - Inteligentný Asistent (GTK4 Version)

Pokročilý AI asistent napísaný v C++ s použitím **GTK4/gtkmm4** - kompletne zadarmo a bez licenčných obmedzení! Dokáže komunikovať, učiť sa a generovať kód.

## 🆓 **Prečo GTK4 namiesto Qt6?**

- ✅ **Úplne zadarmo** - žiadne licenčné poplatky ani obmedzenia
- ✅ **Open Source** - LGPL licencia bez komerčných obmedzení  
- ✅ **Moderné GUI** - GTK4 je najnovšia verzia s krásnym dizajnom
- ✅ **Cross-platform** - funguje na Linux, Windows, macOS
- ✅ **Ľahšie nasadenie** - žiadne obavy z licenčných problémov

## 🚀 Funkcie

### 🧠 Umelá Inteligencia
- **Neurónová sieť**: Simulácia jednoduchej neurónovej siete pre učenie
- **Rozpoznávanie vzorov**: Automatické rozpoznávanie a kategorizovanie vstupov
- **Samoučenie**: AI sa učí z každej interakcie a zlepšuje svoje odpovede
- **Adaptívne učenie**: Prispôsobuje sa novým vzorcom a situáciám

### 💻 Generovanie Kódu
- **Viacjazyčná podpora**: C++, Python, JavaScript, Java
- **Inteligentné šablóny**: Automatické vyberanie najvhodnejších šablón
- **Syntaxová validácia**: Kontrola správnosti generovaného kódu
- **Optimalizácia kódu**: Základné optimalizácie a formátovanie

### 🌐 Sieťové Funkcie
- **Internetové pripojenie**: Automatická kontrola pripojenia
- **Web vyhľadávanie**: Integrácia s DuckDuckGo API
- **AI API podpora**: Podpora pre OpenAI API (voliteľné)
- **GitHub integrácia**: Sťahovanie kódu z GitHub repozitárov

### 🎨 Moderné GUI (GTK4)
- **Tmavý motív**: Elegantné tmavé rozhranie s CSS3 štýlmi
- **Záložkové rozhranie**: Chat, Kód, Učenie
- **Responzívny dizajn**: Prispôsobuje sa veľkosti okna
- **Slovenská lokalizácia**: Kompletne v slovenskom jazyku

## 📋 Požiadavky

### Systémové požiadavky
- **OS**: Linux, Windows, macOS
- **Kompilátor**: GCC 8+, Clang 10+, alebo MSVC 2019+
- **CMake**: 3.16 alebo novší

### Závislosti (všetky zadarmo!)
- **GTK4**: Moderný GUI toolkit
- **gtkmm4**: C++ wrapper pre GTK4
- **libcurl**: Pre HTTP požiadavky
- **jsoncpp**: Pre JSON spracovanie
- **C++17**: Štandardná knižnica

## 🛠️ Inštalácia

### Ubuntu/Debian (Najjednoduchšie)
```bash
# Automatická inštalácia všetkých závislostí a kompilácia
./build.sh --install-deps --clean --run
```

### Manuálna inštalácia Ubuntu/Debian
```bash
# Inštalácia závislostí
sudo apt update
sudo apt install -y build-essential cmake git pkg-config
sudo apt install -y libgtk-4-dev libgtkmm-4.0-dev
sudo apt install -y libcurl4-openssl-dev libjsoncpp-dev

# Kompilácia
mkdir build && cd build
cmake ..
make -j$(nproc)

# Spustenie
./AIAssistant
```

### Fedora/RHEL/CentOS
```bash
# Inštalácia závislostí
sudo dnf install -y gcc-c++ cmake git pkgconfig
sudo dnf install -y gtk4-devel gtkmm40-devel
sudo dnf install -y libcurl-devel jsoncpp-devel

# Kompilácia
./build.sh --clean
```

### Arch Linux
```bash
# Inštalácia závislostí
sudo pacman -S gcc cmake git pkgconf
sudo pacman -S gtk4 gtkmm-4.0
sudo pacman -S curl jsoncpp

# Kompilácia
./build.sh --clean
```

### Windows (MSYS2)
```bash
# V MSYS2 termináli
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-gtkmm4
pacman -S mingw-w64-x86_64-curl mingw-w64-x86_64-jsoncpp

# Kompilácia
./build.sh --clean
```

### macOS
```bash
# Inštalácia Homebrew (ak nie je nainštalované)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Inštalácia závislostí
brew install cmake pkg-config gtk4 gtkmm4 curl jsoncpp

# Kompilácia
./build.sh --clean
```

## 🎯 Použitie

### Prvé spustenie
1. **Spustite aplikáciu** - Zobrazí sa tmavé GTK4 rozhranie
2. **Internetové pripojenie** - Aplikácia automaticky kontroluje pripojenie
3. **Začnite konverzáciu** - Napíšte správu do chat okna

### Chat rozhranie
```
Používateľ: Ahoj, ako sa máš?
AI Assistant: Ahoj! Som AI asistent. Môžem vám pomôcť s programovaním, 
odpovedať na otázky a učiť sa z našej konverzácie.

Používateľ: Vytvor mi jednoduchý C++ program
AI Assistant: [Generuje C++ kód pre Hello World program]
```

### Generovanie kódu
```
Príklady požiadaviek:
- "Vytvor kalkulačku v C++"
- "Napíš Python funkciu pre triedenie"
- "Generuj JavaScript trieda pre používateľa"
- "Vytvor jednoduchy hello world program"
```

## 🏗️ Architektúra

### Hlavné komponenty

#### AIEngine (`src/AIEngine.cpp`)
- Centrálny mozog aplikácie
- Spracovanie prirodzeného jazyka
- Neurónová sieť a rozhodovanie
- Kontextová pamäť
- **Callback systém** namiesto Qt signals/slots

#### NetworkManager (`src/NetworkManager.cpp`)
- Správa internetového pripojenia
- **libcurl** pre HTTP/HTTPS požiadavky
- API integrácie
- Sťahovanie obsahu

#### CodeGenerator (`src/CodeGenerator.cpp`)
- Generovanie kódu vo viacerých jazykoch
- Šablónový systém
- Syntaxová validácia
- Optimalizácia kódu

#### LearningModule (`src/LearningModule.cpp`)
- Strojové učenie
- Rozpoznávanie vzorov
- Neurónová sieť
- Samooptimalizácia

#### MainWindow (`src/MainWindow.cpp`)
- **GTK4/gtkmm4** grafické používateľské rozhranie
- Event handling
- UI komponenty s CSS štýlmi
- Vizualizácia dát

### Technické detaily
```cpp
// GTK4 widgets namiesto Qt
Gtk::ApplicationWindow     // namiesto QMainWindow
Gtk::TextView             // namiesto QTextEdit
Gtk::Entry               // namiesto QLineEdit
Gtk::Button              // namiesto QPushButton
Gtk::Notebook            // namiesto QTabWidget

// Callback systém namiesto signals/slots
using ResponseCallback = std::function<void(const std::string&)>;
void set_response_callback(ResponseCallback callback);

// libcurl namiesto QNetworkAccessManager
CURL* curl_handle;
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* userp);

// jsoncpp namiesto QJsonDocument
Json::Value root;
Json::Reader reader;
```

## 🔧 Konfigurácia

### Nastavenie API kľúčov
```cpp
// V main.cpp alebo cez GUI
network_manager->set_api_key("your-openai-api-key");
```

### Dátové súbory
Aplikácia ukladá dáta do:
- **Linux**: `~/.local/share/AI_Assistant/`
- **Windows**: `%APPDATA%/AI_Assistant/`
- **macOS**: `~/Library/Application Support/AI_Assistant/`

### Súbory:
- `knowledge.json` - Vedomostná báza AI
- `learning_data.json` - Učebné dáta
- `conversations/` - Uložené konverzácie
- `generated_code/` - Generovaný kód

## 🎨 Prispôsobenie

### CSS štýly (GTK4)
```css
/* V src/main.cpp */
.chat-display {
    background-color: #1e1e1e;
    color: #ffffff;
    font-family: 'Consolas', 'DejaVu Sans Mono', monospace;
}

.send-button {
    background: linear-gradient(to bottom, #0078d4, #106ebe);
    border-radius: 8px;
}
```

### Pridanie nových jazykov
```cpp
// V CodeGenerator::generate_code()
else if (language == "rust" || description.contains("rust")) {
    result.code = generate_rust_code(description);
    result.language = "rust";
}
```

## 🆚 Porovnanie s Qt6 verziou

| Vlastnosť | GTK4 Verzia | Qt6 Verzia |
|-----------|-------------|------------|
| **Licencia** | ✅ Úplne zadarmo | ⚠️ Komerčné obmedzenia |
| **Závislosti** | GTK4, libcurl, jsoncpp | Qt6 (veľké) |
| **Veľkosť** | ~50MB | ~200MB+ |
| **Nasadenie** | Jednoduché | Licenčné problémy |
| **Výkon** | Rýchly štart | Pomalší štart |
| **Vzhľad** | Natívny GTK | Qt štýl |

## 🐛 Riešenie problémov

### Časté problémy

#### GTK4 nie je nájdené
```bash
# Ubuntu/Debian
sudo apt install libgtk-4-dev

# Fedora
sudo dnf install gtk4-devel

# Arch
sudo pacman -S gtk4
```

#### gtkmm4 chýba
```bash
# Ubuntu/Debian
sudo apt install libgtkmm-4.0-dev

# Fedora  
sudo dnf install gtkmm40-devel

# Arch
sudo pacman -S gtkmm-4.0
```

#### Chyby pri kompilácii
```bash
# Skontrolujte verzie
pkg-config --modversion gtk4
pkg-config --modversion gtkmm-4.0

# Aktualizujte systém
sudo apt update && sudo apt upgrade
```

### Debug režim
```bash
# Kompilácia v debug režime
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

# GTK debug informácie
GTK_DEBUG=interactive ./AIAssistant
```

## 📈 Výhody GTK4 verzie

### ✅ **Licenčné výhody**
- Žiadne komerčné poplatky
- Žiadne obmedzenia na distribúciu
- Môžete predávať aplikácie bez problémov
- Open source bez "hidden costs"

### ✅ **Technické výhody**
- Menšie závislosti
- Rýchlejší štart aplikácie
- Natívny vzhľad na Linux
- Lepšia integrácia so systémom

### ✅ **Vývojárske výhody**
- Žiadne obavy z licenčných problémov
- Jednoduchšie nasadenie
- Menšie balíčky na distribúciu
- Viac kontroly nad UI

## 🤝 Prispievanie

Vítame príspevky od komunity! 

### Ako prispieť
1. **Fork** repozitára
2. **Vytvorte branch** pre vašu funkcionalitu
3. **Commitnite** zmeny s popisnými správami
4. **Otestujte** kód na rôznych platformách
5. **Vytvorte Pull Request**

### Kódové štandardy
- **C++17** štandard
- **GTK4/gtkmm4** coding style
- **Komentáre v slovenčine** pre dokumentáciu
- **Unit testy** pre nové funkcionality

## 📄 Licencia

Tento projekt je licencovaný pod MIT licenciou - pozrite súbor [LICENSE](LICENSE) pre detaily.

**GTK4/gtkmm4 sú licencované pod LGPL** - môžete ich používať vo vlastných projektoch bez obmedzení.

## 👨‍💻 Autori

- **Hlavný vývojár** - AI Assistant Team
- **Prispievatelia** - Pozrite [CONTRIBUTORS.md](CONTRIBUTORS.md)

## 🙏 Poďakovanie

- **GTK Project** - Za skvelý GUI framework
- **gtkmm Project** - Za C++ wrapper
- **libcurl** - Za sieťové funkcie
- **jsoncpp** - Za JSON spracovanie
- **Komunita** - Za spätnú väzbu a návrhy

---

**Vytvorené s ❤️ pre slovenskú programátorskú komunitu**

**🆓 Úplne zadarmo - žiadne licenčné problémy!**

Pre viac informácií navštívte našu [dokumentáciu](docs/) alebo kontaktujte tím cez [Issues](../../issues).