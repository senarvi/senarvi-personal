3ds2dat.exe: 3ds2dat.o
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C++ Linker'
	@echo g++  -o3ds2dat.exe 3ds2dat.o
	@g++  -o3ds2dat.exe 3ds2dat.o
	@echo 'Finished building target: $@'
	@echo ' '

%.o: $(ROOT)/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	@echo g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $<
	@g++ -O0 -g3 -Wall -c -fmessage-length=0 -o$@ $< && \
	echo -n $(@:%.o=%.d) $(dir $@) > $(@:%.o=%.d) && \
	g++ -MM -MG -P -w -O0 -g3 -Wall -c -fmessage-length=0  $< >> $(@:%.o=%.d)
	@echo 'Finished building: $<'
	@echo ' '
