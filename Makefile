#the things that tells about the system
#PREF=/usr/local
#EXECUTEPATH=/usr/local/bin
#INST=/games/blockattack

-include Makefile.local

standard:
	@echo "Now compiling..."
	@make -C source/code -f Makefile
	@echo "Everything has been compiled!"
	
clean:
	rm -f ./source/code/build/*.o
	@make -C source/code -f Makefile clean
	rm -f ./source/code/blockattack #Build by scons
	rm -f ./Game/blockattack #build by this
	
