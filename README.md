# Crypgraphy Snippets

This repository holds all of my program submissions for CSC 152 (Cryptography) taken at California State University, Sacramento with Dr. Ted Krovetz. This class covered a wide range of cryptographic principles and involved some moderate programming with the [OpenSSL](https://www.openssl.org/) library.


## Organization

Each program can be found within the folder numbered with the week that program was assigned (i.e. the second assignment was assigned during Week 3 of the course and can be found in the folder numbered ``3``). Each folder contains the instructions for that assignment in ``README.html`` and the the corresponding source files for my submission.


## Compilation

Most submissions require [OpenSSL](https://openssl-library.org/) to compile correctly. Once OpenSSL is installed, you can compile them with your C compiler of choice. An example is shown below for [GCC](https://gcc.gnu.org/) on [Kali Linux](https://www.kali.org/).

> [!NOTE]
> These programs were meant to be submitted for automated grading and therefore most of them didn't include a ``main`` function. Some programs have a ``main`` function that can be enabled from the command line with the ``-DMAIN`` macro and performs some automated testing for correctness.

```bash
# Compiling Program 3 (Week 5).

# Install OpenSSL.
sudo apt update && sudo apt install libssl-dev

# Clone the repo.
git clone https://github.com/PeanutButterRat/cryptography-snippets

# Change to the Program 3 directory.
cd cryptography-snippets/3

# Compile and link against OpenSSL.
gcc -DMAIN -o chat_security chat_security.c -lcrypto -lssl

# Run Program 3.
./chat_security
```

## License

Distributed under the [MIT License](https://choosealicense.com/licenses/mit/). See `LICENSE.txt` for more information.

Uploaded with permission from Dr. Krovetz (tdk@csus.edu).

