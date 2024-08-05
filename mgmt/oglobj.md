# OpenGL Abstraction Objects

Should be lightweight structs that hold meta or helper data for the object. These structs may have helper methods for interacting with the object, but shouldn't be seen as the only way. The renderer id for each object should be publicly accessible.

These structs should have their copy assignment and operator deleted, so that (if used correctly) there aren't duplicates of info objects for the same ogl object, and the metadata should be up-to-date.

These structs should have constructors which take in the rendererID of an already created ogl object. There can be helper methods which create the ogl obj, and then return a pointer to a created info object.

Any mutable metadata which can be changed (within reason) using the rendererID on the ogl side should be public. Anything else should be private.