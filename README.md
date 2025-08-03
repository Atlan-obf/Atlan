# AI Assistant - Inteligentný Asistent

Pokročilý AI asistent napísaný v C++ s použitím Qt6, ktorý dokáže komunikovať, učiť sa a generovať kód.

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

### 🎨 Moderné GUI
- **Tmavý motív**: Elegantné tmavé rozhranie
- **Záložkové rozhranie**: Chat, Kód, Učenie
- **Responzívny dizajn**: Prispôsobuje sa veľkosti okna
- **Slovenská lokalizácia**: Kompletne v slovenskom jazyku

## 📋 Požiadavky

### Systémové požiadavky
- **OS**: Linux, Windows, macOS
- **Kompilátor**: GCC 8+, Clang 10+, alebo MSVC 2019+
- **CMake**: 3.16 alebo novší

### Závislosti
- **Qt6**: Core, Widgets, Network moduly
- **libcurl**: Pre HTTP požiadavky
- **C++17**: Štandardná knižnica

## 🛠️ Inštalácia

### Ubuntu/Debian
```bash
# Inštalácia závislostí
sudo apt update
sudo apt install build-essential cmake git
sudo apt install qt6-base-dev qt6-tools-dev libcurl4-openssl-dev

# Klonovanie repozitára
git clone <repository-url>
cd AIAssistant

# Kompilácia
mkdir build && cd build
cmake ..
make -j$(nproc)

# Spustenie
./AIAssistant
```

### Windows (MSYS2/MinGW)
```bash
# V MSYS2 termináli
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-qt6 mingw-w64-x86_64-curl

# Kompilácia
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make

# Spustenie
./AIAssistant.exe
```

### macOS
```bash
# Inštalácia Homebrew (ak nie je nainštalované)
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Inštalácia závislostí
brew install cmake qt6 curl

# Kompilácia
mkdir build && cd build
cmake ..
make -j$(sysctl -n hw.ncpu)

# Spustenie
./AIAssistant
```

## 🎯 Použitie

### Prvé spustenie
1. **Spustite aplikáciu** - Zobrazí sa uvítacia správa
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

### Učenie a adaptácia
- AI si pamätá predchádzajúce konverzácie
- Učí sa z vašich reakcií a preferencií
- Postupne zlepšuje kvalitu odpovední
- Rozpoznáva nové vzory v komunikácii

## 🏗️ Architektúra

### Hlavné komponenty

#### AIEngine (`src/AIEngine.cpp`)
- Centrálny mozog aplikácie
- Spracovanie prirodzeného jazyka
- Neurónová sieť a rozhodovanie
- Kontextová pamäť

#### NetworkManager (`src/NetworkManager.cpp`)
- Správa internetového pripojenia
- HTTP/HTTPS požiadavky
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
- Grafické používateľské rozhanie
- Event handling
- UI komponenty
- Vizualizácia dát

### Dátové štruktúry
```cpp
// Učebné dáta
struct LearningData {
    QString input;      // Vstup od používateľa
    QString output;     // Odpoveď AI
    QString context;    // Kontext konverzácie
    double reward;      // Hodnotenie kvality
    qint64 timestamp;   // Časová pečiatka
    QString category;   // Kategória vzoru
};

// Šablóna kódu
struct CodeTemplate {
    QString language;       // Programovací jazyk
    QString pattern;        // Rozpoznávací vzor
    QString template_code;  // Šablóna kódu
    QStringList variables;  // Premenné na nahradenie
    QString description;    // Popis šablóny
};
```

## 🔧 Konfigurácia

### Nastavenie API kľúčov
Pre plnú funkcionalitu môžete nastaviť API kľúče:

```cpp
// V main.cpp alebo cez GUI
networkManager->setApiKey("your-openai-api-key");
```

### Dátové súbory
Aplikácia ukladá dáta do:
- **Linux**: `~/.local/share/AI Development Team/AI Assistant/`
- **Windows**: `%APPDATA%/AI Development Team/AI Assistant/`
- **macOS**: `~/Library/Application Support/AI Development Team/AI Assistant/`

### Súbory:
- `knowledge.json` - Vedomostná báza AI
- `learning_data.json` - Učebné dáta
- `conversations/` - Uložené konverzácie
- `generated_code/` - Generovaný kód

## 🎨 Prispôsobenie

### Pridanie nových jazykov
```cpp
// V CodeGenerator::generateCode()
else if (language == "rust" || description.contains("rust")) {
    result.code = generateRustCode(description);
    result.language = "rust";
}
```

### Nové šablóny kódu
```cpp
CodeTemplate rustTemplate;
rustTemplate.language = "rust";
rustTemplate.pattern = "hello.*world";
rustTemplate.template_code = 
    "fn main() {\n"
    "    println!(\"Hello World!\");\n"
    "}";
templates.append(rustTemplate);
```

### Vlastné vzory učenia
```cpp
// V LearningModule::analyzeCategory()
else if (lowerInput.contains("rust")) {
    return "rust_programming";
}
```

## 🐛 Riešenie problémov

### Časté problémy

#### Aplikácia sa nespustí
```bash
# Skontrolujte závislosti
ldd ./AIAssistant  # Linux
otool -L ./AIAssistant  # macOS

# Skontrolujte Qt inštaláciu
qmake --version
```

#### Chyby pri kompilácii
```bash
# Aktualizujte CMake
cmake --version  # Malo by byť 3.16+

# Skontrolujte C++17 podporu
g++ --version  # Malo by byť 8+
```

#### Problémy s pripojením
- Skontrolujte firewall nastavenia
- Overte internetové pripojenie
- Skontrolujte proxy nastavenia

### Debug režim
```bash
# Kompilácia v debug režime
cmake .. -DCMAKE_BUILD_TYPE=Debug
make

# Spustenie s debug výstupom
QT_LOGGING_RULES="*.debug=true" ./AIAssistant
```

## 📈 Budúce vylepšenia

### V pláne
- [ ] **Pokročilejšia neurónová sieť** - Hlboké učenie
- [ ] **Viac programovacích jazykov** - Rust, Go, Swift
- [ ] **Hlasové rozhranie** - Speech-to-text a text-to-speech
- [ ] **Plugin systém** - Rozšíriteľnosť cez pluginy
- [ ] **Databázová podpora** - SQLite integrácia
- [ ] **Cloudová synchronizácia** - Synchronizácia naprieč zariadeniami

### Možné rozšírenia
- **IDE integrácia** - Plugin pre VS Code, Qt Creator
- **Web verzia** - Browser-based verzia
- **Mobilná aplikácia** - Android/iOS verzia
- **API server** - REST API pre externé aplikácie

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
- **Qt coding style** pre Qt špecifický kód
- **Komentáre v slovenčine** pre dokumentáciu
- **Unit testy** pre nové funkcionality

## 📄 Licencia

Tento projekt je licencovaný pod MIT licenciou - pozrite súbor [LICENSE](LICENSE) pre detaily.

## 👨‍💻 Autori

- **Hlavný vývojár** - AI Assistant Team
- **Prispievatelia** - Pozrite [CONTRIBUTORS.md](CONTRIBUTORS.md)

## 🙏 Poďakovanie

- **Qt Framework** - Za skvelý GUI framework
- **OpenAI** - Za inšpiráciu v oblasti AI
- **DuckDuckGo** - Za verejné API
- **Komunita** - Za spätnú väzbu a návrhy

---

**Vytvorené s ❤️ pre slovenskú programátorskú komunitu**

Pre viac informácií navštívte našu [dokumentáciu](docs/) alebo kontaktujte tím cez [Issues](../../issues).