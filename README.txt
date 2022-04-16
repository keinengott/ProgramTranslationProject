Storage=Local
Author: Robert Kaufman
Note: While working on this, I noticed a pretty big mistake in the parser with how I was handling expr and when I fixed that it caused knock-on issues in how I was traversing the tree in the code generator. This whole project became a mess that I could not salvage in time. I submit this here in hopes of a few pity points. As of submitting this, I still haven't received feedback on P2 or P3. I mistakenly operated on the assumption that P2 was working.

To compile, type 'make' in this directory. 'make clean' will remove the generated files.

To run the project, either type /compfs somefile' or './compfs < somefile.fs'
