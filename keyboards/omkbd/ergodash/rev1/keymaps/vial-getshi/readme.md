## setup

- install newest qmk msys
- download newest vial-qmk repo

## to modify / flash firmware:

- open qmk msys
- go to vial repo root
- `make omkbd/ergodash/rev1:vial-getshi`
- flash using qmk_toolbox? (if the right half doesn't respond, may be due to bigger firmware changes. flash non-master separately, then master with other connected again)

## TODO
- simplify base keymap.c more like default vial (+ new custom keycodes) and instead save the custom setup as a .vil json file
- undo RALT on f layer, because that then only works to compose stuff that is on the f layer...