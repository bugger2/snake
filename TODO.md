# Quick hands can bypass the backwards moving blockage

If you time the keys right you can move backwards when you shouldn't.

For example, if the snake is moving left, you can tap up, then right really quickly and the game will let you move right

This happens because the 'up' tap changes `dir` to `UP`, which then lets you change to right freely, without transitioning to an actual block above the snake, since you tap both keys slower than 144fps, but faster than 8 (144/18) fps, so raylib detects both keypresses, but we don't decide to redraw until after the 8fps refresh.