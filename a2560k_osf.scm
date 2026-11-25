(define memories
  '((memory flash (address (#x10000 . #x8ffff)) (type ROM))       ; code and constants: 512 KB (was 256 KB, $10000-$4FFFF)
    (memory dataRAM (address (#x90000 . #x39ffff)) (type RAM))    ; data, BSS, heap, stack: up to MCP's area at $3A0000
    (memory Vector (address (#x0000 . #x03ff)))
    ))
