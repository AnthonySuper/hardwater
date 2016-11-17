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
Each Ion splits itself into a given number of *chunks*, which are small segments of data.
Each chunk is identified by a hair of hashes: the encrypted hash, which is the hash of the data when encrypted to the solution's private key, and the unencrypted hash, which is the hash of the data when it is decrypted.
The Ion itself is identified by a creation time and a file path, as well as a pair of "overall" encrypted and unencrypted hashes.

Ions are transferred chunk by chunk between nodes in the solution, with only encrypted data being transferred.
Ions find each other using a tracker, and then communicate between each other to maximize the availability of each chunk.
If a node with the private key modifies or adds an Ion, it tells the other nodes and transfers the new Ion to all of them.
The other nodes can verify that this information is correct because it is signed with the private key.

I have little idea of what to do if the same Ion is modified at the same time by two different nodes.
In this case it is possible that we can recognize the conflict and create two files with random numbers after their file names, letting the user choose between them.
Alternatively, we could just say that the "last modification wins", which may be a bad idea.


This project is in the early development stages.
