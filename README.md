# osu-tp  
A bug where osu! occasionally renders the cursor outside of the regular monitor bounds  

## Observation Environment 
```
No Monitor to the Left
Above 1.0 Sens
Raw Input
Gamon Tablet Driver in Relative Mode
```
## Settings Explaination  

### Mouse | Sensitivity  
A sensitivity above `1.0` will force "Raw Input" to be enabled.  

### Mouse | [Raw Input](https://osu.ppy.sh/community/forums/topics/187785?n=1)
"Raw input allows osu! to read mouse/tablet values directly from the hardware, without any post-processing. As a result, it can bypas windows bugs and acceleration algorithms. It can also allow for tablet sensitivity adjustment without setting a custom tablet area mapping at driver-level."

### Mouse | Confine Cursor
This setting will allow you to chose when osu! will confine the WINDOWS cursor to the osu! game window.

## Anticipated Behavior  
- Windows does not allow the Windows cursor to move out of the monitor bounds to the left (at all).  
- osu! does not allow the osu! cursor to move out of the monitor bounds to the left (more than half way) depending on certain environmental configurations.  

*Note: The amount moved past the barrier differs because of where the actual center of the sprite is.*  

## Unanticipated Behavior  
- user is able to move the osu! cursor out of monitor bounds more than halfway to the left.
- osu! then teleports the player's cursor back into regular play bounds.

## Proposed Explaination
- There is both an osu! and Windows curosr.
- Due to using a sensitivity above one, the player is forced to use Raw Input.
- Raw Input makes osu! read the tablet driver to determine the osu! cursor position, allowing for it to be set past the bounds typically containing the windows cursor.
- Due to some bug at this process, Gamon will report the osu! cursor to be outside of play, causing osu! to attempt to render it off screen before catching this mistake,

## Additional Tests
`In all tests, there is no monitor present to the left of the monitor displaying osu!`

**Moving the Windows Cursor**
```c++
// if user presses DELETE, move WINDOWS cursor to X -10000, Y
if (GetAsyncKeyState(VK_DELETE) & 1)
{
  POINT p;

  // get the Windows cursor position
  GetCursorPos(&p);

  // statically set the Windows cursor coordinates to -10000
  SetCursorPos(-10000, p.y);
}
```

**Moving the osu! Cursor (External Memory Manipulation)** 
```c++
// if user presses END, move osu! cursor to X -10000, Y
if (GetAsyncKeyState(VK_END) & 1)
{
  POINT p;

  // get the cursor position
  GetCursorPos(&p);

  // set the cursor position to -10000
  Player::SetCursorPosition({ -10000, p.y });
}
```

### Moving the Windows cursor using the Windows API while Raw Input is enabled:
When attempting to use the Windows API to move the osu! cursor using the SetCursorPos(fl,fl) function, notice there is no response from the osu! cursor.
https://github.com/hxruhime/osu-tp/blob/main/assets/windows%20cursor%20raw%20input.gif

### Moving the Windows cursor using the Windows API while Raw Input is disabled:
When attempting to use the Windows API to move the osu! cursor using the SetCursorPos(fl,fl) function, notice the osu! cursor teleports to halfway off the screen horizontally.
https://github.com/hxruhime/osu-tp/blob/main/assets/windows%20cursor%20no%20raw%20input.gif

### Moving the osu! cursor using the Windows API while Raw Input is enabled:

### Moving the osu! cursor using the Windows API while Raw Input is disabled:
