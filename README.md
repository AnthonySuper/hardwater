# Hardwater

Hardwater is a distributed file sharing system, similar to bittorent sync.


## Structure

Hardwater consists of Ions in a Solution.
Each Ion represents a file at a given point in time, and a solution is a collection of such files - essentially, a folder.

Solutions are identified with a public key.
Those with the private key are allowed to change items Ions in the solution, including the ability to remove them.

A given solution can be accessed by many nodes, which store files.
These nodes are typically computers in a network, although they might be mobile devices or hosted servers.

Only nodes with the *private key* of an Ion can actually read the data.
All data which is transferred is encrypted.
However, nodes with the *public key* are still allowed to download and archive Ions---they just can't read the data.
This makes it possible for companies or people to provide server-space for users, while keeping user data secure.

## How it works
Hardwater works by transferring Ions around.
Ions can be added to a solution by anybody with the *solution private key* (nodes which we will now call  "Owner Nodes")
The Ion is split into chunks of equal length.

The adding node then generates a 256-bit random string.
This string is the *Ion Key*.
The node then generates an *encrypted Ion file* using this key.
It does this by encrypting each chunk, taking the hash of the encrypted data, and storing it.
It must also store the *size* of every chunk, as Encryption can create chunks of varying size (note: not sure if this is actually the case, if it isn't this can be skipped, since chunk size is public).
It also encrypts the *Ion Key* with the *Solution Private Key*.
It stores all this information in the format described in the [Ioncrypt file format](#ioncrypt-file-format)

*Dissolution* can now begin.
The node sends the Encrypted Ion File out to all other nodes registered with the public key.
The Ions make contact, and transfer each chunk (encrypted with the ion key) between each other in a way to maximize total availability of each chunk.
Nodes use the hashes to encrypt the files.

Owner nodes can then use their private key to decrypt the *Ion Key*, and then use that *Ion Key* to decrypt the actual data.

# Ioncrypt File Format
An Ioncrypt file is a binary file describing an Ion in a given solution.

- 4-byte header of "NMHW" in ASCII
- 8 bits indicating the current version of the protocol (currently `00000001`).
- 32 bits indicating the length of the encrypted Ion key
- The encrypted Ion Key
- SHA256 Hash of the source file when unencrypted
- 32 bits indicating the length of the Ion's path
- The path, encoded as UTF-8
- 64 bits indicating the UNIX time stamp at which the ion was added
- 16 bits indicating the number of chunks
- A list of encrypted chunks, where each chunk is:
  - 32 bits indicating the index of the chunk, encrypted with the Ion Key
  - 32 bits indicating the length of the chunk data when encrypted
  - Hash of the chunk when encrypted
- 32 bits indicating the length of some application-specific metadata
- Application-specific metadata
- 8 bits of reserved space, filled with '\0'
- An RSA Signature of the preceding data, signed with the *Solution Key*

# Information Splitting
