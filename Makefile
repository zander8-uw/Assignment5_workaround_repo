all:
	for d in HuntTheWumpus UnitTestHuntTheWumpus; \
	do \
		$(MAKE) -j --directory=$$d all; \
	done

clean:
	$(MAKE) -j -C HuntTheWumpus clean
	$(MAKE) -j -C UnitTestHuntTheWumpus clean
