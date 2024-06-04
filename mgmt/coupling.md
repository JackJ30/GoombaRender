# Coupling
## OpenGL
Currently every feature assume that we are using OpenGL. They are mostly held behind abstractions, so it shouldn't be impossible to decouple. The window creation process and OpenGL are tightly coupled.

## SDL3
Big win! (And big loss) SDL3 is not tightly coupled to this project in terms of window creation, but right now I am using SDL for all window events, there is no wrapper. 