# Makefile for Turbo Assembler
# ravage/STATiC 1996, 1997
#
# make -Debug includes debugging information

!if $d(ebug)
ASM_OPTIONS=-la -c -x -zi
LINK_OPTIONS=-m -M -s -l -v
!else
ASM_OPTIONS=
LINK_OPTIONS=
!endif

.asm.obj:
  tasmx $(ASM_OPTIONS) $<

demo.exe:       main.obj pmode.obj init16.obj init32.obj int.obj video.obj\
                math.obj vector.obj object.obj object2.obj\
                phong.obj texmap.obj bumpmap.obj flame.obj tunnel.obj\
                smoke.obj\
                data.obj data2.obj
  tlink $(LINK_OPTIONS) @demo.rsp, demo

main.obj:       main.asm declare.mac video.mac int.mac math.mac system.mac\
                main.inc init.inc int.inc allocate.inc math.inc video.inc kb.inc\
                object.inc vector.inc flame.inc tunnel.inc bumpmap.inc\
                smoke.inc
vector.obj:     vector.asm declare.mac system.mac math.mac vector.mac\
                vector.inc system.inc math.inc allocate.inc
phong.obj:      phong.asm declare.mac vector.mac system.mac opcode.mac\
                vector.inc math.inc allocate.inc video.inc data.inc\
                init.inc
texmap.obj:     texmap.asm declare.mac vector.mac system.mac opcode.mac\
                vector.inc math.inc allocate.inc video.inc data.inc\
                init.inc
bumpmap.obj:    bumpmap.asm declare.mac math.mac bumpmap.inc allocate.inc\
                memory.inc data.inc
flame.obj:      flame.asm declare.mac math.mac flame.inc allocate.inc\
                video.inc
tunnel.obj:     tunnel.asm declare.mac tunnel.inc allocate.inc math.mac\
                vector.inc data.inc
smoke.obj:      smoke.asm declare.mac smoke.inc data.inc allocate.inc
object.obj:     object.asm declare.mac object.inc
object2.obj:    object2.asm declare.mac object.inc
data.obj:       data.asm declare.mac data.inc
data2.obj:      data2.asm declare.mac data.inc

