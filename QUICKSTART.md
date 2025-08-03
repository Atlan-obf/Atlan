# 🚀 Quick Start Guide - AI Assistant

Tento návod vás rýchlo prevedie inštaláciou a spustením AI Assistenta.

## ⚡ Rýchla inštalácia (Ubuntu/Debian)

```bash
# 1. Nainštalujte závislosti
sudo apt update
sudo apt install -y build-essential cmake git qt6-base-dev qt6-tools-dev libcurl4-openssl-dev

# 2. Klonujte projekt (alebo stiahnite ZIP)
git clone <repository-url>
cd AIAssistant

# 3. Kompilujte a spustite
./build.sh --clean --run
```

## 🎯 Prvé kroky po spustení

### 1. Uvítacia obrazovka
Po spustení sa zobrazí uvítacia správa s informáciami o funkcionalite.

### 2. Základné testovanie
Skúste tieto základné príkazy v chat okne:

```
Ahoj!
Ako sa máš?
Vytvor hello world program v C++
Napíš kalkulačku
Generuj Python funkciu
```

### 3. Navigácia v aplikácii
- **💬 Chat tab**: Hlavná komunikácia s AI
- **💻 Kód tab**: Prezeranie a úprava generovaného kódu
- **🧠 Učenie tab**: Sledovanie pokroku učenia AI

## 🔧 Riešenie problémov

### Aplikácia sa nespustí
```bash
# Skontrolujte závislosti
ldd build/AIAssistant

# Skontrolujte Qt inštaláciu
dpkg -l | grep qt6
```

### Chyby pri kompilácii
```bash
# Aktualizujte systém
sudo apt update && sudo apt upgrade

# Preinštalujte Qt6
sudo apt install --reinstall qt6-base-dev qt6-tools-dev
```

### Chýbajúce Qt6 (starší Ubuntu)
```bash
# Pridajte Qt6 PPA
sudo add-apt-repository ppa:okirby/qt6-backports
sudo apt update
sudo apt install qt6-base-dev qt6-tools-dev
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

### Klávesové skratky
- **Enter**: Odoslať správu
- **Ctrl+Enter**: Nový riadok v správe
- **Ctrl+S**: Uložiť konverzáciu
- **Ctrl+L**: Vymazať chat

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
rm -rf ~/.local/share/AI\ Development\ Team/AI\ Assistant/

# Vymazať iba konverzácie
rm -rf ~/.local/share/AI\ Development\ Team/AI\ Assistant/conversations/
```

### Záloha dát
```bash
# Záloha učebných dát
cp -r ~/.local/share/AI\ Development\ Team/AI\ Assistant/ ~/ai_assistant_backup/
```

## 🆘 Podpora

### Kde hľadať pomoc
1. **README.md** - Detailná dokumentácia
2. **Issues** - Nahláste chyby a problémy
3. **Diskusie** - Otázky a návrhy

### Časté otázky

**Q: AI nedáva zmysluplné odpovede**
A: AI potrebuje čas na učenie. Pokračujte v konverzácii a postupne sa zlepší.

**Q: Aplikácia je pomalá**
A: Skontrolujte internetové pripojenie a dostupnú RAM.

**Q: Kód sa nekompiluje**
A: Skontrolujte, či máte nainštalované potrebné kompilátory a knižnice.

**Q: Chcem pridať nový programovací jazyk**
A: Pozrite si sekciu "Prispôsobenie" v README.md

---

**Úspešné spustenie! 🎉**

Teraz môžete začať experimentovať s AI Assistentom. Pamätajte, že AI sa učí z každej interakcie, takže čím viac ho používate, tým lepší sa stane!