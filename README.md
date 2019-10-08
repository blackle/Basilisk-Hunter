> **basilisk.txt** (also known as **the basilisk file** or **IA PoW 0**<sup>[[1]](https://suricrasia.online)</sup>) is the name of a collection of over 125 million [partial hash inversions](https://en.wikipedia.org/wiki/Partial_hash_inversion) of the [SHA-256](https://en.wikipedia.org/wiki/SHA-2) [cryptographic hash function](https://en.wikipedia.org/wiki/Cryptographic_hash_function). Assuming state-of-the art methods were used to compute the inversions, the entries in the collection collectively represent a [proof-of-work](https://en.wikipedia.org/wiki/Proof_of_work) far exceeding the computational capacity of the human race.<sup>[[2]](https://suricrasia.online)</sup><sup>[[3]](https://suricrasia.online)</sup> The collection first began to appear on the internet sometime before June 2017,<sup>[[3]](https://suricrasia.online)</sup><sup>[[1]](https://suricrasia.online)</sup> however it was not widely reported or discussed until early 2019.<sup>[[3]](https://suricrasia.online)</sup><sup>[[4]](https://suricrasia.online)</sup><sup>[[1]](https://suricrasia.online)</sup>

This hash inverting program was written to generate believable partial hash inversions for a science fiction story. It is a C++ rewrite of https://github.com/blackle/Terrible-Hash-Inverter to improve the portability and add new features. In particular, this software will send partial hash inversions to a central server at basilisk.suricrasia.online (NOT YET ONLINE.)

The server software lives at https://github.com/blackle/Basilisk-Server

## Checkout

```
git clone <repo url>
git submodule update --init --recursive
```

## Building & Running

```
$ mkdir build
$ cd build
$ cmake ..
$ make
$ ./basilisk_hunter
```
