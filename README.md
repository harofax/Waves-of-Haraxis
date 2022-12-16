# Waves-of-Haraxis

### CONTROLS

WASD -> movement

SPACE -> shoot

ESC -> exit

---

EXE is in the bin folder

I recommend opening your favourite wallpaper in the background B^) (yes, the transparent background is intentional)

---

In this project I have utilized en Entity Component System to make the most out of a data-oriented approach in making this game.

Having the data separated from the logic allows for a more cache/cpu-friendly approach when processing game logic. 
Having all the components stored in a contiguous array, and processing all those components at once through the systems, reduces the
cache-misses that would happen in an object-oriented application.

For example, when rendering we only care about entities with a Sprite component, a Bounds component, and a Position component. That way,
the rendering system can rapidly process (render) all those in one loop, and have the data be accessible in a cpu-friendly way (hopefully
loading as many as possible into the cache, so it doesn't have to run all the way to the RAM every time).

I went through many different iterations on how to implement this in a way that made sense, but this is where I landed.
Once the boilerplate was set up, it was very fun and fast to develop the gameplay, although due to running out of time I'm not that
happy with the collision I decided to go with.

If I had more time I would consider changing from std::vector storage to something else, perhaps a sparse-set or something. I also
got a lot of headaches from the templates and debugging those templates, but I also learned more about them so that's cool.

Perhaps an ECS was a bit overkill for this project, but I learnt a whole lot and that's good so I don't regret going this path.
