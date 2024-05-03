# CWE<span style="font-size:50%">(ngine)

A 2D game engine built with C++ with Lua bindings with a builtin editor

`<span><img src="docs/license_libre.png" style="width:3%; margin-bottom:-3px">`MIT license

<a style="font-size:120%" href="#1-luaref">Lua Reference</a></br>
<a style="font-size:120%" href="#1-luaexamples">Example Code</a>
# Compiling
The project is made with Visual Studio 2022, using VCPKG. Ensure you have these dependencies before building!

---

<h1 id="1-luaref", style="font-size:300%">Lua Reference</h1>

- [Events](#1-events)

  - [Adding Events](#1-events-adding)
  - [Event Names](#1-events-names)
  - [Keyboard](#1-events-keyboard)
  - [Mouse](#1-events-mouse)
  - [Rendering](#1-events-rendering)
  - [Game Events](#1-events-game-loop)
- [Input](#1-input)

  - [Keyboard](#1-input-keyboard)
    - [KeyCodes](#1-input-keyboard-keycodes)
  - [Mouse](#1-input-mouse)
    - [Clicks]()
    - [Scroll]()
    - [Movement]()
  - [Controller Input (NOT IMPLEMENTED)]()
    - [Gyro input]()

<h1 id="1-events">Events</h1>
<h3 id="1-events-adding">Registering Events</h3>
<p>
- <code>Event.addEventListener(name:string, event:function)</code>

    Adds an event listener under the given event

</p><p>
- <code>Event.removeEventListener(name:string, event:function)</code>

    Removes a previously registered event listener by reference

</p><p>
- <code>Event.removeEventListenersFor(name:string)</code>

    Removes all previously registered event listeners for a given event

</p><p>
- <code>Event.getEventListeners(name:string) -> [...]</code>

    Gets all listeners for a given event.

</p>
<h2 id="1-events-names"> Event Names </h2>
<h3 id="1-events-keyboard">Keyboard Events</h3>
<p>

- `<code>` on_key_press -> (key:int)`</code>` - Fires when a key is pressed.
- `<code>` on_key_hold -> (key:int)`</code>` - Fires while a key is held.
- `<code>` on_key_release -> (key:int)`</code>` - Fires when a key is released.

<h3 id="1-events-mouse">Mouse Events</h3>

- `<code>` on_mouse_click -> (x:int, y:int, left:bool, middle:bool, right:bool)`</code>` - Fires when clicking with a mouse
- `<code>` on_mouse_move -> (x:int, y:int)`</code>` - Fires when moving the cursor
- `<code>` on_mouse_scroll -> (amount_scrolled:float)`</code>` - Fires when scrolling

<h3 id="1-events-rendering">Rendering Events</h3>

- `<code>` on_render -> ()`</code>` - Fires during the render loop
- `<code>` on_gui -> ()`</code>` - Fires during the render loop, but after rendering the scene

<h3 id="1-events-game-loop">Game Events</h3>

These are general purpose game loop, or scene events.

- `<code>` on_update → ()`</code>` - Fires during the update loop (every frame)
- `<code>` on_init → ()`</code>` - Fires when the scene finishes loading assets
- `<code>` on_save → ()`</code>` - Fires when requested to save
- `<code>` on_load → (assets:{})`</code>` - Fires when requested to load assets. The assets will be provided in a JSON-like format.
- `<code>` on_engine_exit → ()`</code>` - Fires before the game finishes exiting (will not occur during any crashes)
- `<code>` on_engine_init → ()`</code>` - Fires when the game first loads (This event must be defined in the autoexec file)

</p>

<h1 id="1-input">Input</h1>
<h3 id="1-input-keyboard">Keyboard Input</h3>
<p>
- <code>key_to_name(keycode:int) → string</code>

    Returns a readable version of the keycode.
	Example: input.key_to_name(input.GLFW_KEY_A) → "a"

</p>
<p>
- <code>key_down(keycode:int) → bool</code>

    Returns if the supplied keycode is being held

</p>
<p>
- <code>key_pressed(keycode:int) → bool</code>

    Returns if the supplied keycode was pressed this frame

</p>
<h4 id="1-input-keyboard-keycodes">Keycodes</h4>
Keycodes use <span style="color:white"><strong>GLFW's Key mappings. </strong></span>

In lua, you can use input.GLFW_KEY_* to get direct key references.
Lua-implemented names can be found below:

<details><summary>KeyCode Mappings</summary>
	<code>input.Key_A = 65</code><br>
	<code>input.Key_B = 66</code><br>
	<code>input.Key_C = 67</code><br>
	<code>input.Key_D = 68</code><br>
	<code>input.Key_E = 69</code><br>
	<code>input.Key_F = 70</code><br>
	<code>input.Key_G = 71</code><br>
	<code>input.Key_H = 72</code><br>
	<code>input.Key_I = 73</code><br>
	<code>input.Key_J = 74</code><br>
	<code>input.Key_K = 75</code><br>
	<code>input.Key_L = 76</code><br>
	<code>input.Key_M = 77</code><br>
	<code>input.Key_N = 78</code><br>
	<code>input.Key_O = 79</code><br>
	<code>input.Key_P = 80</code><br>
	<code>input.Key_Q = 81</code><br>
	<code>input.Key_R = 82</code><br>
	<code>input.Key_S = 83</code><br>
	<code>input.Key_T = 84</code><br>
	<code>input.Key_U = 85</code><br>
	<code>input.Key_V = 86</code><br>
	<code>input.Key_W = 87</code><br>
	<code>input.Key_X = 88</code><br>
	<code>input.Key_Y = 89</code><br>
	<code>input.Key_Z = 90</code><br>
	TODO: paste rest of mappings!
</details>
