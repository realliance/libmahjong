# Shorthand Tile Notation

The following tile notation is used by `Mahjong::HandFromNotation` to produce a list of tiles from a given string. This string is made up of any number of sets of tiles, which `Mahjong::HandFromNotation` will expand into a corresponding list.


A given set of tiles (a set being any number of tiles greater than 1) is defined by digits followed by a suit character. All digits within that set are translated to the corresponding numbered tile with the given set suit. Red Fives corresponds to the digit `0`.

### Suit Table

| Character   | Suit |
| :---------: | ---  |
| `m`         | Man (Character)  |
| `s`         | Sou (Bamboo/Reed) |
| `p`         | Pin |
| `z`         | Honor |

## Honor Tiles

Honor tiles are represented as numbered tiles with the honor (`z`) suit. The following table can be used for translation:

| #   | Tile |
| :------: | ---- |
| 1 | `EAST_WIND` |
| 2 | `SOUTH_WIND` |
| 3 | `WEST_WIND` |
| 4 | `NORTH_WIND` |
| 5 | `WHITE_DRAGON` |
| 6 | `GREEN_DRAGON` |
| 7 | `RED_DRAGON` |

An easy way to remember this order is winds-then-dragons, with each set being in the order used for determining doras.

## `libmahjong` Specifics

`Mahjong::HandFromNotation` holds compliance with this notation. If invalid inputs are given, `Mahjong::HandFromNotation` will translate them to piece `ERROR_PIECE` to stay aligned with other piece functions. If an individual tile is invalid (such as giving a symbol in place of a number), this individual piece will be translated to `ERROR_PIECE`. If a letter is found that does not correspond to any value on the suit label, it is considered an invalid set and all tiles in that set are translated to `ERROR_PIECE`.

## Examples

```
123m -> ONE_CHARACTER, TWO_CHARACTER, THREE_CHARACTER

1m1m -> ONE_CHARACTER, ONE_CHARACTER

1p2m3s -> ONE_PIN, TWO_CHARACTER, THREE_BAMBOO

1234z -> EAST_WIND, SOUTH_WIND, WEST_WIND, NORTH_WIND

777z -> RED_DRAGON, RED_DRAGON, RED_DRAGON

111m055s789p555z66z ->

ONE_CHARACTER, ONE_CHARACTER, ONE_CHARACER,
RED_FIVE_BAMBOO, FIVE_BAMBOO, FIVE_BAMBOO,
SEVEN_PIN, EIGHT_PIN, NINE_PIN,
WHITE_DRAGON, WHITE_DRAGON, WHITE_DRAGON,
GREEN_DRAGON, GREEN_DRAGON
```

## Refereneces

Original Notation Reference

[Is there a standard notation for tiles?](https://www.reddit.com/r/Mahjong/comments/dgth5z/is_there_a_standard_notation_for_tiles/)

Ordering of Honors

[EUMA Riichi Mahjong Rules 2016, §2.7 The dora indicator](http://mahjong-europe.org/portal/images/docs/Riichi-rules-2016-EN.pdf)