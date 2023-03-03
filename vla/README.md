# vla

This reproducer highlihts an issue when building some code not supported on GPU, but where no compilation error is thrown, causing the code to fail at runtime without explanation.

In particular, we show that declaring variable length arrays into a device region compiles perfectly but is not supported. Interestingly enough, when the variable used to declare the VLA is itself declared with a value known at compilation in the same function, the compiler is able to replace it with that value. The array becomes static, and the code works correctly. 

In previous `nvc` versions (<23.1), this used to prevent any device code to be embedded the same executable. This behaviour has now improved, but we would still expect a compialtion warning/error.
