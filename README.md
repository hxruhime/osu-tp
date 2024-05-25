# osu-tp  
A behavior where osu! occasionally renders / allows the cursor outside of the regular monitor bounds due to what is believed to be a bugged interaction between the tablet driver and the "Raw Input" setting within osu!.  
  
More specifically the "Gamon" tablet driver was speculated to be the source of this, but SIMILAR behavior may be produced through using an XP-PEN tablet with no driver. You're not actually able to play using no driver unlike when using the Gamon driver, but it seems worth noting.

- [IN AND OUT OF GAME | Clip of this thought to be happening to "worst hr player"](https://streamable.com/pssvvk)
- [IN GAME | Clip of "Shiratori-P" thought to be recreating this behavior](https://www.youtube.com/watch?v=OTuvFCODsbY)

***TLDR***  
When typically not possible, sometimes a player may move their cursor out of bounds due to what is thought to be an inaccurate interaction between the tablet driver and osu! while the "Raw Input" setting is enabled. This can cause players that experience this bug to look like they may have external software interacting with their osu! game process.  

- [IN GAME | Demonstration of recreating this behavior ARTIFICALLY through use of "maple.software"](https://streamable.com/itnxj6)

It is still not 100% that this behavior is due to osu!, Gamon, or other unkown / unconsidered factors, but so far this seems to be the most logical explaination. More testing is required, but the entire off screen and teleportation visually related portion of this is likely the osu! process writing it's cursor to invalid coords, realizing it's mistake, then setting the cursor back in play.  

It is also not known if being in or out of game (playstate as in menu vs mid map) has any affect on this behavior, but it seems more than likely it does not.  

| Tablet            | Driver           | Behavior Occurs |
| :---------------- | :--------------: | :-----: |
| Gamon ?           | Gamon Driver     | [✔️](https://www.youtube.com/watch?v=OTuvFCODsbY) |
| XP-PEN Star G630s | PenTablet        | [❌](https://youtu.be/n3PcbDo6324)                |
| XP-PEN Star G630s | None             | [⚠️](https://youtu.be/PUQRm2EHhRE)                |
| Wacom CTL - 480   | OpenTabletDriver | [❌](https://youtu.be/AZfd7HCz8T0)                |

*Please keep in mind this is the product of only two days of research, and that research into this possible bug has been completely independant. All work done in this repository is only for public information, and none of it's authors or contributors are leaning in any direction regarding any topic when it comes to its use other than the plain fact that this game behavior appears to POSSIBLY be unanticipated. This information, its authors, and its contributors are not claiming or intending to claim any proven fact. Only speculation.*

## Observation Environment 
```
No Monitor to the Side Being Penetrated
Above 1.0 Sensitivity
Raw Input
Gamon Tablet Driver
Relative / Mouse Mode
```
# osu! Settings Explaination  

##### Mouse | Sensitivity  
###### A sensitivity above `1.0` will force "Raw Input" to be enabled.  
###### It is currently unknown if this makes a difference.  

##### Mouse | [Raw Input](https://osu.ppy.sh/community/forums/topics/187785?n=1)
###### "Raw input allows osu! to read mouse/tablet values directly from the hardware, without any post-processing. As a result, it can bypas windows bugs and acceleration algorithms. It can also allow for tablet sensitivity adjustment without setting a custom tablet area mapping at driver-level."
###### This appears to be related to the main cause of the issue.

##### Mouse | Confine Cursor
###### This setting will allow you to chose when osu! will confine the WINDOWS cursor to the osu! game window.
###### It is currently unknown if this makes a difference.  

## Behavior Anticipated to Occur  
- Windows does not allow the Windows cursor to move out of the monitor bounds where no monitor is present.  
- osu! does not allow the osu! cursor to move out of the monitor bounds to the where no monitor is present (more than half way) depending on certain environmental configurations.  

*Note: The amount moved past the barrier differs because of where the actual center of the sprite is.*  

# Unanticipated Behavior Occuring  
- user is able to move the osu! cursor out of monitor bounds more than halfway to where no monitor is present.
- osu! then teleports the player's cursor back into regular play bounds.

# Proposed Explaination
- There is both an osu! and Windows curosr.
- Due to using a sensitivity above one, the player is forced to use Raw Input.
- Raw Input makes osu! read the tablet driver to determine the osu! cursor position, allowing for it to be set past the bounds typically containing the windows cursor.
- Due to some bug at this process, Gamon (or other tablet drivers?) will report the osu! cursor to be outside of play, causing osu! to attempt to render it off screen before catching this mistake,

# Additional Tests by Software
`In all tests done by software, there is no monitor present to the left of the monitor displaying osu !and confine cursor is always set to "Never".`

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

  // get the Windows cursor position
  GetCursorPos(&p);

  // set the osu! cursor position to -10000
  Player::SetCursorPosition({ -10000, p.y });
}
```

## OUT OF GAME | Moving the Windows cursor using the Windows API while Raw Input is enabled:
When attempting to use the Windows API to move the osu! cursor using the SetCursorPos(fl,fl) function, notice there is no response from the osu! cursor.  
https://cdn.discordapp.com/attachments/731235337073852436/1243736240868687982/windows_cursor_raw_input.gif?ex=66528f23&is=66513da3&hm=95ea739862db5df355f10d2e8ec112e2aadde12b8e5fb688b43e259b8e3615f7&

## OUT OF GAME | Moving the Windows cursor using the Windows API while Raw Input is disabled:
When attempting to use the Windows API to move the osu! cursor using the SetCursorPos(fl,fl) function, notice the osu! cursor teleports to halfway off the screen horizontally.  
https://cdn.discordapp.com/attachments/731235337073852436/1243736255481774161/windows_cursor_no_raw_input.gif?ex=66528f26&is=66513da6&hm=da0df6a4826d13fcaf87689c794e28c4d2468a313038fb07cf009d67aea4131c&
