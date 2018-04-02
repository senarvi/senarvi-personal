
;----------------------------------------------------------------------------
        struc   smemstats
free    dd      ?
        dd      11 dup (?)
        ends

;----------------------------------------------------------------------------
_interrupt_vector_table         =       00000000h
_BIOS_data_area                 =       00000400h
_data_area                      =       00000500h
_DOS_scrach_space               =       00000600h
_IOSYS_load_address             =       00000700h
_VGA_graphics_buffer            =       000a0000h
_VGA_text_buffer                =       000b8000h
_VGA_video_BIOS                 =       000c0000h
_hard_disk_BIOS                 =       000c8000h
_ROM_basic                      =       000f6000h
_reset_jump                     =       000ffff0h
_BIOS_date                      =       000ffff5h
_machine_type_code              =       000ffffeh
_high_memory_area               =       00100000h
_weitek_coprocessor             =      0c0000000h
_IO_APIC                        =      0fec00000h
_local_APIC                     =      0fee00000h

_BIOS_COM1_base_address         =       00000400h
_BIOS_COM2_base_address         =       00000402h
_BIOS_COM3_base_address         =       00000404h
_BIOS_COM4_base_address         =       00000406h
_BIOS_LPT1_base_address         =       00000408h
_BIOS_LPT2_base_address         =       0000040ah
_BIOS_LPT3_base_address         =       0000040ch
_BIOS_LPT4_base_address         =       0000040eh
_BIOS_installed_hardware        =       00000410h
_BIOS_self_test_status          =       00000412h
_BIOS_memory_size               =       00000413h
_BIOS_keyboard_control          =       00000417h
_BIOS_alternate_keypad          =       00000419h
_BIOS_keyboard_buffer_head      =       0000041ah
_BIOS_keyboard_buffer_tail      =       0000041ch
_BIOS_keyboard_buffer           =       0000041eh
_BIOS_floppy_disk_info          =       0000043eh
_BIOS_display_mode              =       00000449h
_BIOS_video_columns             =       0000044ah
_BIOS_regen_buffer_length       =       0000044ch
_BIOS_regen_buffer_address      =       0000044eh
_BIOS_cursor_position           =       00000450h
_BIOS_cursor_type               =       00000460h
_BIOS_display_page              =       00000462h
_BIOS_video_base_address        =       00000463h
_BIOS_mode_select_reg           =       00000465h
_BIOS_palette_value_reg         =       00000466h
_BIOS_reset_code_pointer        =       00000467h
_BIOS_timer_counter             =       0000046ch
_BIOS_timer_overflow_flag       =       00000470h
_BIOS_break_key_state           =       00000471h
_BIOS_reset_flag                =       00000472h
_BIOS_fixed_disk_info           =       00000473h
_BIOS_LPT1_timeout              =       00000478h
_BIOS_LPT2_timeout              =       00000479h
_BIOS_LPT3_timeout              =       0000047ah
_BIOS_LPT4_timeout              =       0000047bh
_BIOS_COM1_timeout              =       0000047ch
_BIOS_COM2_timeout              =       0000047dh
_BIOS_COM3_timeout              =       0000047eh
_BIOS_COM4_timeout              =       0000047fh
_BIOS_keyboard_buffer_start     =       00000480h
_BIOS_keyboard_buffer_end       =       00000482h
_BIOS_video_rows                =       00000484h
_BIOS_char_height               =       00000485h
_BIOS_video_info                =       00000487h
_BIOS_media_control             =       0000048bh

_BIOS_drive_info                =       0000048ch
_BIOS_keyboard_info             =       00000496h
_BIOS_user_wait_complete        =       00000498h
_BIOS_user_wait_count           =       0000049ch
_BIOS_wait_active_flag          =       000004a0h
_BIOS_print_screen_status       =       00000500h

_PSP_int_20                     =       00h
_PSP_next_paragraph             =       02h
_PSP_dispatcher                 =       05h
_PSP_terminate_vector           =       0ah
_PSP_controlc_vector            =       0eh
_PSP_crit_error_vector          =       12h
_PSP_environment                =       2ch
_PSP_FCB_1                      =       48h
_PSP_FCB_2                      =       58h
_PSP_command_tail               =       6ch

