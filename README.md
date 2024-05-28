# wideriver

Tiling window manager for the [river](https://github.com/riverwm/river) wayland compositor, inspired by [dwm](https://dwm.suckless.org/) and [xmonad](https://xmonad.org/)

- Per-tag state
- Master/stack(s) left, right and wide layouts
- Monocle with alternate borders
- Layout toggling
- Dwindling, diminishing and even stacks
- Inner, outer and smart gaps

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
- [GAPS](#gaps)
- [INSTALL](#install)
  * [Package Manager](#package-manager)
  * [From Source](#from-source)
- [USAGE](#usage)
- [OPTIONS](#options)
- [COMMANDS](#commands)
- [RECIPES](#recipes)
  * [Wide Shuffling](#wide-shuffling)
- [FAQ](#faq)
  * [Name Does Not Always Update](#name-does-not-always-update)
  * [Borders Are Not Shown](#borders-are-not-shown)
  * [Random Pixels In Gaps When Using Fractional Scaling](#random-pixels-in-gaps-when-using-fractional-scaling)
- [ISSUES](#issues)
  * [Problems](#problems)
  * [Ideas](#ideas)
  * [Questions or Discussions](#questions-or-discussions)
  * [Contributions](#contributions)

<!-- tocstop -->

## QUICK START

`$XDG_CONFIG_HOME/river/init`

<br/>

<details>
<summary>
Set server side decorations.
</summary>

Required to display borders on all windows.

``` sh
riverctl rule-add ssd
```

</details>

<br/>

<details>
<summary>
Set the layout generator and start it.
</summary>

All defaults shown here, none are required.

Log file is strongly recommended.

``` sh
# set layout manager
riverctl default-layout wideriver

# start layout manager
wideriver \
    --layout                       left        \
    --layout-alt                   monocle     \
    --stack                        dwindle     \
    --count-master                 1           \
    --ratio-master                 0.50        \
    --count-wide-left              0           \
    --ratio-wide                   0.35        \
    --no-smart-gaps                            \
    --inner-gaps                   0           \
    --outer-gaps                   0           \
    --border-width                 2           \
    --border-width-monocle         0           \
    --border-width-smart-gaps      0           \
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

## GAPS

Gaps, in pixels, between windows may be injected. They are off by default.

`--inner-gaps` (`i`) are between windows.

`--outer-gaps` (`o`) are between the edge of the screen and windows.

`--smart-gaps` automatically hides the gaps when there is only one view or monocle layout.

`--border-width-smart-gaps` the border width for when smart gaps hides the gaps, excluding monocle layout. For a seamless experience, set this to the same value as `--border-width-monocle`

     ---------------------------------------------------------- 
    | o    o    o    o    o    o    o    o    o    o    o    o |
    |    ----------------------     -----------------------    |
    | o |                      | i |                       | o |
    |   |                      |   |                       |   |
    | o |                      | i |                       | o |
    |   |                      |    -----------------------    |
    | o |                      | i   i   i   i   i   i   i   o |
    |   |                      |    -----------------------    |
    | o |                      | i |                       | o |
    |   |                      |   |                       |   |
    | o |                      | i |                       | o |
    |    ----------------------     -----------------------    |
    | o    o    o    o    o    o    o    o    o    o    o    o |
     ---------------------------------------------------------- 

## INSTALL

### Package Manager

[![Packaging status](https://repology.org/badge/vertical-allrepos/wideriver.svg)](https://repology.org/project/wideriver/versions)

### From Source

[![CI](https://github.com/alex-courtis/wideriver/actions/workflows/ci.yml/badge.svg?branch=master)](https://github.com/alex-courtis/wideriver/actions/workflows/ci.yml?query=branch%3Amaster)

See [CONTRIBUTING.md](CONTRIBUTING.md)

``` sh
git clone clone git@github.com:alex-courtis/wideriver.git
cd wideriver
make
sudo make install
```

Should install under `/usr/local`

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

      --(no-)smart-gaps
      --inner-gaps                    pixels                                0           0 <= gap size
      --outer-gaps                    pixels                                0           0 <= gap size

      --border-width                  pixels                                2           0 <= width
      --border-width-monocle          pixels                                0           0 <= width
      --border-width-smart-gaps       pixels                                0           0 <= width

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

`--(no-)smart-gaps`  
Automatically hides the gaps when there is only one view or monocle layout.

`--inner-gaps` *pixels*  
Inner gaps width, default `0`, minimum `0`.

`--outer-gaps` *pixels*  
Outer gaps width, default `0`, minimum `0`.

`--border-width` *pixels*  
Border width for all layouts except monocle, default `2`, minimum `0`.

`--border-width-monocle` *pixels*  
Border width for monocle layout, default `0`, minimum `0`.

`--border-width-smart-gaps` *pixels*  
Border width for when smart gaps hides the gaps for all layouts except monocle, default `0`, minimum `0`. Has no effect if `--no-smart-gaps`.

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

This may be resolved with a river enhancement: [\#1004](https://github.com/riverwm/river/issues/1002)

### Borders Are Not Shown

Please ensure you have enabled server side decorations i.e. the borders:

``` sh
riverctl rule-add ssd
```

You can still use client side decorations for specific applications e.g.:

``` sh
riverctl rule-add -app-id audacity csd
```

### Random Pixels In Gaps When Using Fractional Scaling

This is a known river and wlroots issue: https://codeberg.org/river/river/issues/816

Workaround: set a river background colour other than default black `0x000000`

`riverctl background-color "0x010101"` is sufficiently close.

## ISSUES

### Problems

Please raise a [Bug Report](https://github.com/alex-courtis/wideriver/issues/new?assignees=&labels=bug&projects=&template=bug_report.yml)

### Ideas

Please create a [Feature Request](https://github.com/alex-courtis/wideriver/issues/new?assignees=&labels=feature&projects=&template=feature_request.yml)

### Questions or Discussions

Please raise an [Issue](https://github.com/alex-courtis/wideriver/issues/new)

### Contributions

[CONTRIBUTING.md](doc/CONTRIBUTING.md) is most gratefully appreciated.
