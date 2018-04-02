
                include declare.mac
                decstart
                decpmcode
                endpmcode
                deccode16
                endcode16
                deccode32
                endcode32
                decstack
                endstack

                include object.inc
                include video.inc
                include polygon.inc
                include misc.inc
                include flame.inc


_interrupt_vector_table         equ     00000000h
_BIOS_data_area                 equ     00000400h
_data_area                      equ     00000500h
_DOS_scrach_space               equ     00000600h
_IOSYS_load_address             equ     00000700h
_VGA_graphics_buffer            equ     000a0000h
_VGA_text_buffer                equ     000b8000h
_VGA_video_BIOS                 equ     000c0000h
_hard_disk_BIOS                 equ     000c8000h
_ROM_basic                      equ     000f6000h
_reset_jump                     equ     000ffff0h
_BIOS_date                      equ     000ffff5h
_machine_type_code              equ     000ffffeh
_high_memory_area               equ     00100000h
_weitek_coprocessor             equ    0c0000000h
_IO_APIC                        equ    0fec00000h
_local_APIC                     equ    0fee00000h

_BIOS_COM1_base_address         equ     00000400h
_BIOS_COM2_base_address         equ     00000402h
_BIOS_COM3_base_address         equ     00000404h
_BIOS_COM4_base_address         equ     00000406h
_BIOS_LPT1_base_address         equ     00000408h
_BIOS_LPT2_base_address         equ     0000040ah
_BIOS_LPT3_base_address         equ     0000040ch
_BIOS_LPT4_base_address         equ     0000040eh
_BIOS_installed_hardware        equ     00000410h
_BIOS_self_test_status          equ     00000412h
_BIOS_memory_size               equ     00000413h
_BIOS_keyboard_control          equ     00000417h
_BIOS_alternate_keypad          equ     00000419h
_BIOS_keyboard_buffer_head      equ     0000041ah
_BIOS_keyboard_buffer_tail      equ     0000041ch
_BIOS_keyboard_buffer           equ     0000041eh
_BIOS_floppy_disk_info          equ     0000043eh
_BIOS_display_mode              equ     00000449h
_BIOS_video_columns             equ     0000044ah
_BIOS_regen_buffer_length       equ     0000044ch
_BIOS_regen_buffer_address      equ     0000044eh
_BIOS_cursor_position           equ     00000450h
_BIOS_cursor_type               equ     00000460h
_BIOS_display_page              equ     00000462h
_BIOS_video_base_address        equ     00000463h
_BIOS_mode_select_reg           equ     00000465h
_BIOS_palette_value_reg         equ     00000466h
_BIOS_reset_code_pointer        equ     00000467h
_BIOS_timer_counter             equ     0000046ch
_BIOS_timer_overflow_flag       equ     00000470h
_BIOS_break_key_state           equ     00000471h
_BIOS_reset_flag                equ     00000472h
_BIOS_fixed_disk_info           equ     00000473h
_BIOS_LPT1_timeout              equ     00000478h
_BIOS_LPT2_timeout              equ     00000479h
_BIOS_LPT3_timeout              equ     0000047ah
_BIOS_LPT4_timeout              equ     0000047bh
_BIOS_COM1_timeout              equ     0000047ch
_BIOS_COM2_timeout              equ     0000047dh
_BIOS_COM3_timeout              equ     0000047eh
_BIOS_COM4_timeout              equ     0000047fh
_BIOS_keyboard_buffer_start     equ     00000480h
_BIOS_keyboard_buffer_end       equ     00000482h
_BIOS_video_rows                equ     00000484h
_BIOS_char_height               equ     00000485h
_BIOS_video_info                equ     00000487h
_BIOS_media_control             equ     0000048bh

_BIOS_drive_info                equ     0000048ch
_BIOS_keyboard_info             equ     00000496h
_BIOS_user_wait_complete        equ     00000498h
_BIOS_user_wait_count           equ     0000049ch
_BIOS_wait_active_flag          equ     000004a0h
_BIOS_print_screen_status       equ     00000500h

_PSP_int_20                     equ     00h
_PSP_next_paragraph             equ     02h
_PSP_dispatcher                 equ     05h
_PSP_terminate_vector           equ     0ah
_PSP_controlc_vector            equ     0eh
_PSP_crit_error_vector          equ     12h
_PSP_environment                equ     2ch
_PSP_FCB_1                      equ     48h
_PSP_FCB_2                      equ     58h
_PSP_command_tail               equ     6ch


RADIX_BITS      equ     4
RADIX_SIZE      equ     1 shl RADIX_BITS

_freebasemem    equ     SSEG+STACKLENGTH

_allocstart     equ     00000000h
_screenbuf      equ     _allocstart
_zbuf           equ     _screenbuf   + GRAPHHEIGHT*GRAPHWIDTH
_radixbuf       equ     _zbuf        + GRAPHWIDTH*GRAPHHEIGHT*4
_faceorder      equ     _radixbuf    + 2*(RADIX_SIZE*MAX_FACES*4)
_rotatetab      equ     _faceorder   + MAX_FACES*4
_normaltab      equ     _rotatetab   + MAX_VERTICES*size svect
_pnormaltab     equ     _normaltab   + MAX_FACES*size sface
_sqrttab        equ     _pnormaltab  + MAX_FACES*size sface
_ledgebuf       equ     _sqrttab     + (MAX_SQRT+1)*4
_redgebuf       equ     _ledgebuf    + GRAPHHEIGHT*EDGESIZE*4
_palette        equ     _redgebuf    + GRAPHHEIGHT*EDGESIZE*4
_wavetab        equ     _palette     + GRAPHCOLORS*3
_flamebuf       equ     _wavetab     + GRAPHWIDTH*GRAPHHEIGHT
_tunneldepth    equ     _flamebuf    + FLAMEBUFFER_X*FLAMEBUFFER_Y
_allocend       equ     _tunneldepth + GRAPHWIDTH*GRAPHHEIGHT*4

MEMREQ          equ     (_allocend+00000fffh) and 0fffff000h
STACKLENGTH     equ     40h

