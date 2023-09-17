import base64
import sys

file = sys.argv[1]

# Open file in binary mode and read its content
with open(file, 'rb') as f:
    file_content = f.read()

# Encode the bytes to base64
base64_bytes = base64.b64encode(file_content)

# Convert the bytes back to a string
base64_string = base64_bytes.decode("utf-8")

print(f"{base64_string}")
