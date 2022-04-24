Add length calc of a vec as a function to make it more atomar
Add angle calc between two vecs as a function to make it more atomar
Add scalar product between two vecs as a function to make it more atomar

Make the distance calc of a length (vel, pos) a template

Before doing the tasks above, it is better to have a general 2d struct, not for vel & pos different once. Maybe you can use typdef/using to differentiate....
.... To that the struct should have the property "length", which can be setted once for vel and pos.