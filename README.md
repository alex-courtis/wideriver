# wideriver

Tiling window manager for the [river](https://github.com/riverwm/river) wayland compositor, inspired by [dwm](https://dwm.suckless.org/) and [xmonad](https://xmonad.org/)

- Per-tag state
- Master/stack(s) left, right and wide layouts
- Monocle with alternate borders
- Layout toggling
- Dwindling, diminishing and even stacks

| Layout  | Symbol    | Master | Stack  | Directions  |
|---------|-----------|--------|--------|-------------|
| Left    | `│ ├─┤`   | Left   | Right  | Down, Right |
| Right   | `├─┤ │`   | Right  | Left   | Down, Left  |
| Top     | `├─┬─┤`   | Top    | Bottom | Right, Down |
| Bottom  | `├─┴─┤`   | Bottom | Top    | Right, Up   |
| Wide    | `├─┤ ├─┤` | Mid    | Left   | Up, Left    |
|         |           |        | Right  | Down, Right |
| Monocle | `│ n │`   | All    | \-     | \-          |

<!-- toc -->

- [QUICK START](#quick-start)
- [EXAMPLE LAYOUTS AND STACKS](#example-layouts-and-stacks)
  * [Left, Dwindle](#left-dwindle)
  * [Right, Even](#right-even)
  * [Wide, Diminish](#wide-diminish)
  * [Monocle](#monocle)
- [LAYOUTS](#layouts)
  * [Left / Right](#left--right)
  * [Wide](#wide)
  * [Monocle](#monocle-1)
- [STACK ARRANGEMENTS](#stack-arrangements)
  * [Even](#even)
  * [Diminish](#diminish)
  * [Dwindle](#dwindle)
- [USAGE](#usage)
- [OPTIONS](#options)
- [COMMANDS](#commands)
- [RECIPES](#recipes)
  * [Wide Shuffling](#wide-shuffling)
- [FAQ](#faq)
  * [Name Does Not Always Update](#name-does-not-always-update)

<!-- tocstop -->

## QUICK START

`$XDG_CONFIG_HOME/river/init`

<br/>

<details>
<summary>
Set the layout generator and start it.
</summary>

All defaults shown here but not required.

``` sh
wideriver \
    --layout                       left        \
    --layout-alt                   monocle     \
    --stack                        dwindle     \
    --count-master                 1           \
    --ratio-master                 0.50        \
    --count-wide-left              0           \
    --ratio-wide                   0.35        \
    --border-width                 2           \
    --border-width-monocle         0           \
    --border-color-focused         "0x93a1a1"  \
    --border-color-focused-monocle "0x586e75"  \
    --border-color-unfocused       "0x586e75"  \
    --log-threshold                info        \
   > "/tmp/wideriver.${XDG_VTNR}.${USER}.log" 2>&1 &
```

</details>

<br/>

<details>
<summary>
Create some command mappings e.g.
</summary>

``` sh
riverctl map normal $mod1 up    send-layout-cmd wideriver "--layout monocle"
riverctl map normal $mod1 down  send-layout-cmd wideriver "--layout wide"
riverctl map normal $mod1 left  send-layout-cmd wideriver "--layout left"
riverctl map normal $mod1 right send-layout-cmd wideriver "--layout right"

riverctl map normal $mod1 Space send-layout-cmd wideriver "--layout-toggle"

riverctl map normal $mod1 plus  send-layout-cmd wideriver "--ratio +0.025"
riverctl map normal $mod1 equal send-layout-cmd wideriver "--ratio 0.35"
riverctl map normal $mod1 minus send-layout-cmd wideriver "--ratio -0.025"

riverctl map normal $mod2 plus  send-layout-cmd wideriver "--count +1"
riverctl map normal $mod2 equal send-layout-cmd wideriver "--count 1"
riverctl map normal $mod2 minus send-layout-cmd wideriver "--count -1"

riverctl map normal $mod2 e     send-layout-cmd wideriver "--stack even"
riverctl map normal $mod2 w     send-layout-cmd wideriver "--stack dwindle"
riverctl map normal $mod2 i     send-layout-cmd wideriver "--stack diminish"
```

</details>

## EXAMPLE LAYOUTS AND STACKS

### Left, Dwindle

`riverctl send-layout-cmd wideriver "--layout left --stack dwindle --count 1 --ratio 0.5"`

              Master                          Stack
    ____________________________________________________________
    |                            |                             |
    |                            |                             |
    |                            |              2              |
    |                            |                             |
    |                            |                             |
    |           1                |_____________________________|
    |                            |              |              |
    |                            |              |      4       |
    |                            |      3       |______________|
    |                            |              |      |       |
    |                            |              |  5   |   6   |
    |____________________________|______________|______|_______|

The master area occupies half of the available space and contains one view.

The stack is filled in a diminishing manner in a downward then rightward directions.

2 may be moved into the master area with `riverctl send-layout-cmd wideriver "--count +1"` and will be evenly placed below 1.

### Right, Even

`riverctl send-layout-cmd wideriver "--layout right --stack even --count 2 --ratio 0.55"`

                Stack                       Master              
    ____________________________________________________________
    |                         |                                |
    |                         |                                |
    |             3           |                1               |
    |_________________________|                                |
    |                         |                                |
    |                         |________________________________|
    |             4           |                                |
    |_________________________|                                |
    |                         |                2               |
    |                         |                                |
    |             5           |                                |
    |_________________________|________________________________|

The master area occupies 55% of the available width and contains two views split evenly.

The stack is split evenly and is filled in a downwards direction.

### Wide, Diminish

`riverctl send-layout-cmd wideriver "--layout wide --stack diminish --count 3 --ratio 0.4"`

              Left Stack               Master                     Right Stack           
    ________________________________________________________________________________
    |          1           |                               |                       |
    |______________________|                               |                       |
    |                      |                               |          5            |
    |          2           |                               |                       |
    |                      |                               |_______________________|
    |______________________|                               |                       |
    |                      |             4                 |          6            |
    |                      |                               |                       |
    |                      |                               |_______________________|
    |          3           |                               |          7            |
    |                      |                               |_______________________|
    |______________________|_______________________________|__________8____________|

The master area occupies 40% of the available with and contains one view.

The left stack contains 3 views, the right stack the remainder.

The left and right stacks each occupy 30% of the available width.

5 may be moved into the master area with `riverctl send-layout-cmd wideriver "--count +1"`. 4 will be placed at the “top” of the stack, below 3.

### Monocle

`riverctl send-layout-cmd wideriver "--layout monocle"`

    ____________________________________________________________
    |                                                          |
    |                                                          |
    |                                                          |
    |                                                          |
    |                                                          |
    |                            1                             |
    |                             2                            |
    |                              3                           |
    |                               4                          |
    |                                5                         |
    |                                 6                        |
    |__________________________________________________________|

Only the currently focused view will be visible.

## LAYOUTS

The symbol is the layout name which may be shown in a status bar such as [Waybar](https://github.com/Alexays/Waybar)’s [river/layout](https://github.com/Alexays/Waybar/wiki/Module:-River#layout) module.

Dynamic settings are available via COMMANDS

*ratio* and *count* are persisted per tag and shared by all layouts except wide, which has its own values.

*stack* is persisted per tag and shared by all layouts.

When multiple tags are focused, the state is persisted for only the lowest tag.

### Left / Right

One master area occupying the full height of the available area with a stack area to the left or right.

*ratio* is the proportion of the available area occupied by master.

*count* is the number of evenly evenly stacked views in the master area.

Left: `│ ├─┤` when *count* \> 0 otherwise `│├──┤`

Right: `├─┤ │` when *count* \> 0 otherwise `├──┤│`

### Wide

One master area occupying the full height of the available area with a stack area to the left and the right.

*ratio* is the proportion of the available area occupied by master. Stacks occupy half of the remaining area.

*count* is the number of views in the left stack.

Master is centred when there are left and right stacks, otherwise it expands into the area that would be occupied the empty stacks.

`├─┤ ├─┤` when *count* \> 0 otherwise `││  ├─┤`

### Monocle

Only one view is focused, occupying all of the available space.

`│ n │` with `n` showing number of views only when greater than 1.

## STACK ARRANGEMENTS

3 arrangements are available for the stack area. It is persisted per tag and applied to all layouts for that tag. See above for an example of each arrangement.

Stacks follow one or two directions determined by the layout.

### Even

This is the “traditional” arrangement with uniformly sized stack views.

Arranged in a column or row in the first stack direction only.

### Diminish

Arranged in a column or row in the first stack direction only.

Height or width diminishes according to the view’s position in the stack:

`2p / (n^2 + n)`

`n` number of views in the stack

`p` position in the stack

### Dwindle

Arranged in a dwindling manner alternating in both stack directions.

Each view occupies half the available / remaining area.

## USAGE

    Usage: wideriver [OPTIONS...|COMMANDS...]

    OPTIONS, startup:

      --layout                        monocle|left|right|top|bottom|wide    left
      --layout-alt                    monocle|left|right|top|bottom|wide    monocle

      --stack                         even|diminish|dwindle                 dwindle

      --count-master                  count                                 1           0 <= count
      --ratio-master                  ratio                                 0.50      0.1 <= ratio <= 0.9

      --count-wide-left               count                                 0           0 <= count
      --ratio-wide                    ratio                                 0.35      0.1 <= ratio <= 0.9

      --border-width                  pixels                                2           0 <= width
      --border-width-monocle          pixels                                0           0 <= width

      --border-color-focused          0xRRGGBB[AA]                          0x93a1a1
      --border-color-focused-monocle  0xRRGGBB[AA]                          0x586e75
      --border-color-unfocused        0xRRGGBB[AA]                          0x586e75

      --help
      --log-threshold                 debug|info|warning|error              info
      --version

    COMMANDS, sent via riverctl(1):

      --layout                        monocle|left|right|top|bottom|wide
      --layout-toggle 

      --stack                         diminish|dwindle|dwindle

      --count                         [+-]count                                         0 <= count
      --ratio                         [+-]ratio                                       0.1 <= ratio <= 0.9

## OPTIONS

`--layout` `monocle`\|`left`\|`right`\|`top`\|`bottom`\|`wide`  
Initial layout, default `left`.

`--layout-alt` `monocle`\|`left`\|`right`\|`top`\|`bottom`\|`wide`  
Initial alternate layout, default `monocle`. Use `--layout-toggle` to switch to alternate layout.

`--stack` `even`\|`diminish`\|`dwindle`  
Initial stacking method, default `dwindle`.

`--count-master` *count*  
Initial number of views in the master area, default `1`, minimum `0`. Does not apply to wide layout.

`--ratio-master` *ratio*  
Initial proportion of the width or height the master area occupies, default `0.5`, minimum `0.1`, maximum `0.9`. Does not apply to wide layout.

`--count-wide-left` *count*  
Initial number of views in the wide layout’s left stack area, default `1`, minimum `0`. You may wish to set this to 0 for a more natural or intuitive feel when launching the first two views.

`--ratio-wide` *ratio*  
Initial proportion of the width the wide layout’s master area occupies, default `0.35`, minimum `0.1`, maximum `0.9`. The default value is best suited to ultrawide monitors, a value of `0.5` may be more useful for 16:9 monitors.

`--border-width` *pixels*  
Border width for all layouts except monocle, default `2`, minimum `0`.

`--border-width-monocle` *pixels*  
Border width for monocle layout, default `0`, minimum `0`.

`--border-color-focused` `0x`*RRGGBB*\[*AA*\]  
Border color for focused views in all layouts excluding monocle, default `0x93a1a1`.

`--border-color-focused-monocle` `0x`*RRGGBB*\[*AA*\]  
Border color for focused view in monocle layout, default `0x586e75`. It is recommended to set this to the unfocused color or a darker colour as an always focused border can be distracting.

`--border-color-unfocused` `0x`*RRGGBB*\[*AA*\]  
Border color for unfocused views in all layouts, default `0x586e75`. Does not apply for monocle layout.

`--log-threshold` `debug`\|`info`\|`warning`\|`error`  
Minimum log level, default `info`.

## COMMANDS

When multiple tags are focused, the command is applied to and persisted for only the lowest tag.

`--layout` `monocle`\|`left`\|`right`\|`top`\|`bottom`\|`wide`  
Set layout persistently for the tag, updating the alternate layout.

`--layout-toggle`  
Set layout to the alternate (previous) for the tag.

`--stack` `diminish`\|`dwindle`\|`dwindle`  
Set stacking method persistently for the tag. Applies to all layouts for the tag.

`--count` \[`+-`\]*count*  
Increment, decrement or set the master count, minimum `0`. For wide layout this is instead the left stack count. Discrete value for wide and all other layouts are persisted per tag. Prefix with `+` to increment, `-` to decrement, or an absolute value.

`--ratio` \[`+-`\]*pixels*  
Increase, decrease or set the master ratio: the proportion of the width or height the master area occupies, minimum `0.1`, maximum `0.9`. Discrete tiling and wide values persisted per tag. Prefix with `+` to increase, `-` to decrease, or an absolute value.

## RECIPES

### Wide Shuffling

You can “shuffle” views through master, focusing the new master using:

`riverctl send-layout-cmd wideriver '--count +1' && riverctl focus-view next"`

`riverctl send-layout-cmd wideriver '--count -1' && riverctl focus-view previous"`

## FAQ

### Name Does Not Always Update

The layout name will not update when there are no views for the selected tags. This can occurs when setting a tag with no views or changing the layout for a tag with no views.

This will require a change to river or a questionable workaround.
