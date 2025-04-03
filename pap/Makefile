CC=gcc
CP=g++
BUILDDIR:=build
TESTDIR:=computer_enhance/perfaware
TEST2DIR:=test

hav_gen: hav_gen.c hav_formula.h pcg64.h
	$(CC) -g $^ -lm -o $(BUILDDIR)/$@.out

parser: jsonparser.c
	$(CC) -g $^ -o $(BUILDDIR)/$@.out

ostimer: listing_0071_os_timer_main.cpp listing_0070_platform_metrics.cpp
	$(CP) -g $^ -o $(BUILDDIR)/$@.out

cputimer: listing_0072_cpu_timer_main.cpp listing_0070_platform_metrics.cpp
	$(CP) -g $^ -o $(BUILDDIR)/$@.out

gcputimer: listing_0073_cpu_timer_guessfreq_main.cpp listing_0070_platform_metrics.cpp
	$(CP) -g $^ -o $(BUILDDIR)/$@.out

clean: 
	rm -r build/*
