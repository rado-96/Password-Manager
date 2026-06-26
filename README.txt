=== Password Manager ===

A secure password manager developed in C++, which allows users to safely generate, store, retrieve and manage passwords through a console-based interface. It features a master password authentication, AES-256 encryption, PBKDF2 key derivation and an encrypted vault storage which stores the user's entries.

## Features:
- Master password authentication.
- Master password hashing using SHA-256.
- PBKDF2-HMAC-SHA256 key derivation.
- AES-256 encryption and decryption.
- Encrypted vault storage.
- Automated date tracking for stored entries.
- Secure memory clearing.
- JSON-based vault serialisation.

## Tech Stack:
- Programming language: C++.
- Operating System: Linux, Windows.
- Compiler: g++.
- Library: Crypto++, JSON.

## Threat Model:
This application is designed to protect stored credentials from unauthorised access through encrypted storage and authentication controls.

## How To Run:

### Requirements:
- C++ compatible compiler (g++).
- Crypto++ library.
- JSON library.
- Linux or Windows operating system.

* On first launch, the program will send a request to generate the Master Password. This password will be used to access the Password Manager functions!
* The program automatically creates the 'master.hash', 'salt.bin' and 'vault.enc' during the first-time setup.

### Running on Linux:

1. Install dependencies (skip this step if already installed):
   sudo apt update
   sudo apt install libcrypto++-dev
   sudo apt update
   sudo apt install nlohmann-json3-dev
   sudo apt update
   sudo apt install g++

2. Compile the program:
   g++ *.cpp -std=c++20 -Iinclude -I/usr/include/cryptopp -lcryptopp -lpthread -o PasswordManager

3. Run the program:
   ./PasswordManager

### Running on Windows:

1. Download the Crypto++ library from the official website: https://www.cryptopp.com/

	1.1. Open the 'cryptopp' folder and run the 'cryptlib.vcxproj' file.

	1.2. Set up the configuration to 'Release' 'x64', and navigate to the Solution Explorer. Right-click on the 'cryptlib' file, and select "Build".

	1.3. After the Solution Build is successful navigate to the 'cryptopp' folder. Open the folders titled 'x64', 'Output', 'Release' in this exact order. The actual Crypto++ library 	     which will be implemented into the program can be found in the 'Release' folder.

	1.4. Open the 'Password Manager.sln'

	1.5. In the Solution Explorer, right-click on the project file (NOT the solution), and select 'Properties'.

	1.6. Select 'C/C++', then 'Additional Include Directories' and upload the 'cryptopp' library folder.

	1.7. Under the 'C/C++' dropdown, select 'Code Generation' and change the 'Runtime Library' to 'Multi-threaded(/MT)'.

	1.8. Select the 'Linker' dropdown, and select 'General'. Then select 'Additional Library Directories' and include the folder which contains the actual Crypto++ library, which was 	     created earlier (e.g. the 'Release' folder).

	1.9. In the 'Linker' dropdown, select 'Input' and in 'Additional Dependencies' include the actual Crypto++ library, which is named 'cryptlib.lib'. 

	1.10. Apply the changes.

2. Download the json.hpp file (just the single file) from the latest release from the JSON GitHub repo: https://github.com/nlohmann/json

	2.1. Create a new folder (name it 'include'), and move the 'json.hpp' file to that folder.

	2.2. In the Solution Explorer, right-click on the project file, and select 'Properties'.

	2.3. Under the 'C/C++' dropdown, select 'General', and then 'Additional Include Directories'.

	2.4. Click 'Edit' and add '&(ProjectDir)include' (without the quotations). 

	2.5. Apply the changes. 