#pragma once

#include <thread>
#include <iostream>
#include <Windows.h>

#include "process.hh"

///////////////////////////////////////////////// memory variables //////////////////////////////////////////////////
namespace Pointers
{
    static uintptr_t Player;
    static uintptr_t Cursor;
    static uintptr_t Gamefield;
}

namespace Addresses
{
    static auto Player = 0;
    static auto Cursor = 0;
    static auto Gamefield = 0;
}

namespace osu
{
    static Process osu = Process::Process("osu!.exe");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////// utility classes  ///////////////////////////////////////////////////
class Vector2 {
public:
	float x, y;

    __forceinline Vector2 operator+(const Vector2& v) const {
        return {
            x + v.x,
            y + v.y
        };
    }

    __forceinline Vector2 operator-(const Vector2& v) const {
        return {
            x - v.x,
            y - v.y
        };
    }

    __forceinline Vector2 operator/(float f) const {
        return {
            x / f,
            y / f
        };
    }

    __forceinline Vector2 operator*(float f) const {
        return {
            x * f,
            y * f
        };
    }
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////// game classes ///////////////////////////////////////////////////////
namespace Player
{
    static void SetCursorPosition(Vector2 vPosition){}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////// main function //////////////////////////////////////////////////////
int main()
{
  // get pointers

  // read addresses 

  // log keybinds now that program has gathered required data
  std::cout << "Press INSERT to exit the program" << std::endl;
  std::cout << "Press DELETE to move the WINDOWS cursor to X -10000" << std::endl;
  std::cout << "Press END to move the OSU cursor to X -10000" << std::endl;
  
  // infinite loop
  while (true)
  {

    // if user presses INSERT, exit the program
    if (GetAsyncKeyState(VK_INSERT) & 1)
    {
	      exit(0);
    }

    // if user presses DELETE, get the WINDOWS cursor position, move WINDOWS cursor to X -10000, Y
    if (GetAsyncKeyState(VK_DELETE) & 1) 
    {
		  POINT p; GetCursorPos(&p);
      SetCursorPos(-10000, p.y);
    }

    // if user presses END, get the WINDOWS cursor position, move OSU cursor to X -10000, Y
    if (GetAsyncKeyState(VK_END) & 1)
    {
      POINT p; GetCursorPos(&p);
      Player::SetCursorPosition({ -10000, p.y });
    }

    // sleep for 1 millisecond to avoid high CPU usage
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

}
  
