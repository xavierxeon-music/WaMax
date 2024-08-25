# POT16

## System 

### init

* global 
```
ct {}
```

* code
```
for i = 0, 15 do
    ct[i] = {r = 0, g = 0, b = 0}
    led_color(i, 1, ct[i].r, ct[i].g, ct[i].b)
    led_value(i, 1, 255)
    led_animation_rate(i, 1, 0)
end
```
### utility

* code
```
page_load(0)
```

### midirx

* local
```
ch midi.ch
c midi.cmd
i midi.p1
v midi.p2
```

* code
```
if c == 128 then
    ct[i].r = 2 * v
    led_color(i, 1, ct[i].r, ct[i].g, ct[i].b)
elseif c == 144 then
    ct[i].g = 2 * v
    led_color(i, 1, ct[i].r, ct[i].g, ct[i].b)
elseif c == 160 then
    ct[i].b = 2 * v
    led_color(i, 1, ct[i].r, ct[i].g, ct[i].b)
elseif c == 208 then
    for i = 0, 15 do
        ct[i] = {r = 0, g = 0, b = 0}
        led_color(i, 1, ct[i].r, ct[i].g, ct[i].b)
        led_value(i, 1, 255)
        led_animation_rate(i, 1, 0)
    end
end
```

## Element 0 - 16

### init

* pot meter mode
```
12 bit, 16383
```

### potmeter

* local
```
ch 2
cc 0-16
```

* midi 14
```
ch num val
```