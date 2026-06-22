# bateria
Universal battery discharge alerting tool for Linux, written in C.

## Usage
The program doesn't provide notifications by itself to preserve its universality (and because I'm lazy), but it can be paired with `notify-send`

```
bateria [--target <percentage> --command <command to run>]...
```

### Example
```
bateria \
--target 25 \
--command "notify-send --urgency=critical 'Battery at 25%'" \
\
--target 1 \
--command "notify-send --urgency=critical '1%. Goodbye cruel world'"
```
