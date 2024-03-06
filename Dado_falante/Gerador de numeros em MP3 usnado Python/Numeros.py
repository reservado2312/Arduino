from gtts import gTTS
import os

# Números de 1 a 10 em texto
numeros = ["um", "dois", "três", "quatro", "cinco", "seis", "sete", "oito", "nove", "dez"]

# Loop pelos números
for i, numero in enumerate(numeros, start=1):
    # Cria um objeto gTTS com o texto do número e a voz masculina em português
    tts = gTTS(text=numero, lang='pt-br', tld='com.br')
    # Salva o arquivo de áudio em formato MP3
    tts.save(f"numero_{i}.mp3")

    print(f"Arquivo de áudio para o número {i} salvo")

# Lista os arquivos gerados
os.system("dir /b *.mp3")
