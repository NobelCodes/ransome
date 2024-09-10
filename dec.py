import os
from cryptography.fernet import Fernet
exclude = ["main.py","dec.py","secret.key","test.txt"]
with open("secret.key","rb") as thekey:
	key=thekey.read()
for file in os.listdir():
	if file not in exclude:
		if os.path.isfile(file):
			with open(file, "rb")as f:
				content = f.read()
			content_decrypted = Fernet(key).decrypt(content)
			with open(file, "wb") as f:
				f.write(content_decrypted)
print("files decrypted")