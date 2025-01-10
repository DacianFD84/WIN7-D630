import pyttsx3

# Funcție pentru a curăța numele fișierului
def sanitize_filename(filename):
    return re.sub(r'[^\w\s-]', '', filename).strip().replace(' ', '_')

# Introducere de date de la utilizator
text = input("Introduceți textul pe care doriți să-l convertiți în voce: ")
language = input("Introduceți codul limbii (ex: 'en' pentru engleză, 'ro' pentru română): ")
output_file = input("Introduceți numele fișierului de ieșire (fără extensie): ")

# Curățare și adăugare extensie .mp3
output_file = sanitize_filename(output_file) + ".mp3"

# Inițializarea motorului pyttsx3
engine = pyttsx3.init()

# Setarea limbii și a vocii
voices = engine.getProperty('voices')
for voice in voices:
    if language in voice.languages:
        engine.setProperty('voice', voice.id)
        break

# Salvarea fișierului audio
engine.save_to_file(text, output_file)
print(f"Fișierul MP3 a fost salvat ca {output_file}")

# Redare fișier
play = input("Doriți să redați fișierul MP3 generat? (da/nu): ").lower()
if play in ['da', 'yes']:
    try:
        playsound(output_file)
    except Exception as e:
        print(f"A apărut o problemă la redare: {e}")
else:
    print("Redarea fișierului a fost omisă.")
