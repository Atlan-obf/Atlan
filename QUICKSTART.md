# 🚀 Quick Start Guide - AI Assistant (GTK4 Version)

Tento návod vás rýchlo prevedie inštaláciou a spustením AI Assistenta s **GTK4** - úplne zadarmo!

## ⚡ Super rýchla inštalácia (Ubuntu/Debian)

```bash
# Jeden príkaz pre všetko!
./build.sh --install-deps --clean --run
```

## 🆓 Prečo GTK4?

- ✅ **Úplne zadarmo** - žiadne licenčné poplatky
- ✅ **Bez obmedzení** - môžete predávať aplikácie
- ✅ **Moderné GUI** - krásne tmavé rozhranie
- ✅ **Rýchle** - menšie závislosti, rýchlejší štart

## 📦 Manuálna inštalácia

### Ubuntu/Debian
```bash
# 1. Závislosti
sudo apt update
sudo apt install -y build-essential cmake git pkg-config
sudo apt install -y libgtk-4-dev libgtkmm-4.0-dev
sudo apt install -y libcurl4-openssl-dev libjsoncpp-dev

# 2. Kompilácia
mkdir build && cd build
cmake ..
make -j$(nproc)

# 3. Spustenie
./AIAssistant
```

### Fedora/RHEL
```bash
# 1. Závislosti
sudo dnf install -y gcc-c++ cmake git pkgconfig
sudo dnf install -y gtk4-devel gtkmm40-devel
sudo dnf install -y libcurl-devel jsoncpp-devel

# 2. Build
./build.sh --clean --run
```

### Arch Linux
```bash
# 1. Závislosti
sudo pacman -S gcc cmake git pkgconf gtk4 gtkmm-4.0 curl jsoncpp

# 2. Build
./build.sh --clean --run
```

### macOS
```bash
# 1. Homebrew
brew install cmake pkg-config gtk4 gtkmm4 curl jsoncpp

# 2. Build
./build.sh --clean --run
```

## 🎯 Prvé kroky po spustení

### 1. Tmavé GTK4 rozhranie
Po spustení sa zobrazí elegantné tmavé rozhranie s CSS3 štýlmi.

### 2. Základné testovanie
Skúste tieto príkazy v chat okne:

```
Ahoj!
Vytvor hello world v C++
Napíš kalkulačku
Generuj Python funkciu
Ako funguje neurónová sieť?
```

### 3. Navigácia v aplikácii
- **💬 Chat tab**: Hlavná komunikácia s AI
- **💻 Kód tab**: Prezeranie a úprava generovaného kódu  
- **🧠 Učenie tab**: Sledovanie pokroku učenia AI

## 🔧 Riešenie problémov

### GTK4 nie je nájdené
```bash
# Ubuntu/Debian
sudo apt install libgtk-4-dev

# Fedora
sudo dnf install gtk4-devel

# Arch
sudo pacman -S gtk4

# Skontrolujte verziu
pkg-config --modversion gtk4
```

### gtkmm4 chýba
```bash
# Ubuntu/Debian
sudo apt install libgtkmm-4.0-dev

# Fedora
sudo dnf install gtkmm40-devel

# Arch
sudo pacman -S gtkmm-4.0

# Skontrolujte verziu
pkg-config --modversion gtkmm-4.0
```

### Starší Ubuntu (20.04, 18.04)
```bash
# GTK4 nie je dostupné v starších verziách
# Aktualizujte na Ubuntu 22.04+ alebo použite Flatpak

# Alternatívne - kompilácia GTK4 zo zdrojov
wget https://download.gnome.org/sources/gtk/4.0/gtk-4.0.0.tar.xz
tar -xf gtk-4.0.0.tar.xz
cd gtk-4.0.0
meson build
ninja -C build install
```

### Windows (MSYS2)
```bash
# V MSYS2 termináli
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake
pacman -S mingw-w64-x86_64-gtk4 mingw-w64-x86_64-gtkmm4
pacman -S mingw-w64-x86_64-curl mingw-w64-x86_64-jsoncpp

./build.sh --clean
```

## 🎨 Prvé použitie

### Generovanie kódu
1. Prejdite na **💻 Kód** tab
2. Kliknite na **🤖 Generovať kód**
3. Alebo napíšte do chatu: "Vytvor C++ program"

### Učenie AI
AI sa automaticky učí z vašich interakcií:
- Pozorujte pokrok v **🧠 Učenie** tabe
- AI si pamätá vaše preferencie
- Postupne zlepšuje kvalitu odpovední

### Internetové funkcie
- Aplikácia automaticky kontroluje pripojenie
- Zelený indikátor = pripojené
- Červený indikátor = offline režim

## 📱 Ukážkové scenáre

### Scenár 1: Začiatočník v programovaní
```
Používateľ: Ahoj, som začiatočník. Môžeš mi vysvetliť programovanie?
AI: Ahoj! Rád vám pomôžem...

Používateľ: Vytvor mi môj prvý program
AI: [Generuje Hello World program s vysvetlením]
```

### Scenár 2: Pokročilý programátor
```
Používateľ: Potrebujem kalkulačku s pokročilými funkciami
AI: [Generuje sofistikovanú kalkulačku]

Používateľ: Pridaj podporu pre komplexné čísla
AI: [Rozširuje kód o komplexné čísla]
```

### Scenár 3: Učenie sa nového jazyka
```
Používateľ: Chcem sa naučiť Python, ukáž mi základy
AI: [Generuje Python príklady s vysvetlením]

Používateľ: Teraz to isté v JavaScript
AI: [Prevedie príklady do JavaScript]
```

## 🎓 Tipy a triky

### Pre lepšie výsledky
- **Buďte špecifickí**: "Vytvor kalkulačku s GUI" vs "Vytvor program"
- **Používajte kontext**: AI si pamätá predchádzajúcu konverzáciu
- **Experimentujte**: Skúšajte rôzne formulácie

### GTK4 špecifické funkcie
- **Tmavý motív**: Automaticky detekovaný
- **CSS štýlovanie**: Prispôsobiteľné vzhľady
- **Natívny vzhľad**: Integrácia so systémom
- **Rýchly štart**: Menšie závislosti

### Pokročilé funkcie
- **Kontextové učenie**: AI sa prispôsobuje vášmu štýlu
- **Šablóny kódu**: Automatické rozpoznávanie vzorov
- **Multi-jazyčnosť**: Podpora pre C++, Python, JS, Java

## 🔄 Aktualizácie a údržba

### Aktualizácia kódu
```bash
git pull origin main
./build.sh --clean
```

### Vymazanie dát
```bash
# Vymazať učebné dáta (resetuje AI)
rm -rf ~/.local/share/AI_Assistant/

# Vymazať iba konverzácie
rm -rf ~/.local/share/AI_Assistant/conversations/
```

### Záloha dát
```bash
# Záloha učebných dát
cp -r ~/.local/share/AI_Assistant/ ~/ai_assistant_backup/
```

## 🆘 Podpora

### Kde hľadať pomoc
1. **README.md** - Detailná dokumentácia
2. **Issues** - Nahláste chyby a problémy
3. **Diskusie** - Otázky a návrhy

### Časté otázky

**Q: Prečo GTK4 namiesto Qt6?**
A: GTK4 je úplne zadarmo bez licenčných obmedzení, menšie závislosti, rýchlejší štart.

**Q: AI nedáva zmysluplné odpovede**
A: AI potrebuje čas na učenie. Pokračujte v konverzácii a postupne sa zlepší.

**Q: Aplikácia je pomalá**
A: GTK4 verzia by mala byť rýchlejšia. Skontrolujte internetové pripojenie a RAM.

**Q: Kód sa nekompiluje**
A: Skontrolujte, či máte nainštalované GTK4, gtkmm4, libcurl a jsoncpp.

**Q: Chcem pridať nový programovací jazyk**
A: Pozrite si sekciu "Prispôsobenie" v README.md

**Q: Môžem predávať aplikácie vytvorené s týmto nástrojom?**
A: Áno! GTK4 a všetky závislosti sú pod LGPL licenciou - žiadne obmedzenia.

## 🆚 Porovnanie s Qt6 verziou

| Vlastnosť | GTK4 Verzia | Qt6 Verzia |
|-----------|-------------|------------|
| **Licencia** | ✅ Zadarmo | ⚠️ Komerčné poplatky |
| **Veľkosť** | ~50MB | ~200MB+ |
| **Štart** | Rýchly | Pomalší |
| **Vzhľad** | Natívny GTK | Qt štýl |
| **Nasadenie** | Jednoduché | Licenčné problémy |

## 🎉 Výhody GTK4 verzie

### ✅ **Licenčné**
- Žiadne poplatky
- Žiadne obmedzenia na predaj
- Open source bez "skrytých nákladov"

### ✅ **Technické**
- Menšie závislosti
- Rýchlejší štart
- Lepšia integrácia so systémom
- Moderné CSS3 štýlovanie

### ✅ **Vývojárske**
- Žiadne obavy z licencií
- Jednoduchšie nasadenie
- Menšie distribučné balíčky

---

**Úspešné spustenie! 🎉**

**🆓 Teraz môžete začať experimentovať s AI Assistentom - úplne zadarmo a bez licenčných obmedzení!**

Pamätajte, že AI sa učí z každej interakcie, takže čím viac ho používate, tým lepší sa stane!