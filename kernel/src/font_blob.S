;https://sourceware.org/binutils/docs/as/Section.html
.section .rodata.font, "a", @progbits
.global font_u_vga16_start
.global font_u_vga16_end

font_u_vga16_start:
    .incbin "assets/u_vga16.sfn"
font_u_vga16_end:
