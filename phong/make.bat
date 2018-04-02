\prog\asm\tasm\tasm /ml /m2 /q /z *
rem \prog\asm\tasm\tasm /ml /m2 /q /z s-t
rem \prog\asm\tasm\tasm /ml /m2 /q /z t-i
rem \prog\asm\tasm\tasm /ml /m2 /q /z a-c
rem \prog\asm\tasm\tasm /ml /m2 /q /z texmap
rem \prog\asm\tasm\tasm /ml /m2 /q /z misc
rem \prog\asm\tasm\tasm /ml /m2 /q /z main
rem \prog\asm\tasm\tasm /ml /m2 /q /z init
rem \prog\asm\tasm\tasm /ml /m2 /q /z video
rem \prog\asm\tasm\tasm /ml /m2 /q /z flame
rem \prog\asm\tasm\tasm /ml /m2 /q /z tunnel
\prog\asm\tasm\tlink /3 /x main pmode dos init misc video int vector rotate s-t t-i a-c duck polygon flame tunnel data, demo
if errorlevel==0 demo
rem mode co80
